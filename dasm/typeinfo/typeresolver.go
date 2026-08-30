package typeinfo

import (
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/nb09"
)

type typeResolver struct {
	ts         nb09.TypeStream
	records    map[uint16]nb09.TypeRecord
	types      map[uint16]Type
	namedTypes map[string]Type
	resolving  map[uint16]bool
}

func newTypeResolver(ts nb09.TypeStream) *typeResolver {
	r := &typeResolver{
		ts:         ts,
		records:    make(map[uint16]nb09.TypeRecord, len(ts.Records)),
		types:      make(map[uint16]Type, len(ts.Records)),
		namedTypes: make(map[string]Type),
		resolving:  make(map[uint16]bool, len(ts.Records)),
	}
	for i, rec := range ts.Records {
		r.records[ts.BaseIndex+uint16(i)] = rec
	}
	for i := range ts.Records {
		r.resolve(ts.BaseIndex + uint16(i))
	}

	// build a mapping of types by their names
	return r
}

func (r *typeResolver) getType(typind uint16) Type {
	return r.resolve(typind)
}

func (r *typeResolver) getNamedType(name string) Type {
	return r.namedTypes[name]
}

// registerType adds a type by name to the resolver
// this is used by overrides to resolve overridden types
func (r *typeResolver) registerNamedType(name string, t Type) {
	r.namedTypes[name] = t
}

func (r *typeResolver) resolve(typind uint16) Type {
	if typ, ok := r.types[typind]; ok {
		return typ
	}
	if typind < r.ts.BaseIndex {
		typ := primitiveType(typind)
		r.types[typind] = typ
		return typ
	}

	rec, ok := r.records[typind]
	if !ok {
		typ := unknownType(typind)
		r.types[typind] = typ
		return typ
	}
	if r.resolving[typind] {
		return unknownType(typind)
	}

	r.resolving[typind] = true
	typ := r.convertRecord(typind, rec)
	r.resolving[typind] = false
	if typ != nil {
		// we don't add arglists or fieldlists to the types map
		r.types[typind] = typ
	}
	return typ
}

func (r *typeResolver) convertRecord(typind uint16, rec nb09.TypeRecord) Type {
	switch v := rec.Parsed.(type) {
	case *nb09.LFModifier:
		return r.resolve(v.UType)
	case *nb09.LFPointer:
		p := &Pointer{
			Class: ptrClassFromAttr(v.Attr),
		}
		r.types[typind] = p
		p.Elem = r.resolve(v.UType)
		return p
	case *nb09.LFArray:
		elem := r.resolve(v.ElemType)
		return &Array{
			Elem:  elem,
			Count: arrayCount(v.Length, elem),
		}
	case *nb09.LFProcedure:
		return &Function{
			Conv:   callConvFromNB09(v.CallType),
			Ret:    r.resolve(v.RetType),
			Params: r.loadParams(v.ArgList),
		}
	case *nb09.LFStruct:
		s := &Struct{
			Name:  v.Name,
			SKind: StructKindStruct, // all nb09 types are structs
			Size:  int(v.Size.Value),
		}
		r.types[typind] = s
		s.Fields = r.loadFields(v.FieldList)
		s.FinalizeLayout()
		return s
	case *nb09.LFBitfield:
		return r.resolve(v.BaseType)
	default:
		return nil
	}
}

func (r *typeResolver) loadParams(argListTypind uint16) []FunctionVar {
	rec, ok := r.records[argListTypind]
	if !ok {
		return nil
	}
	argList, ok := rec.Parsed.(*nb09.LFArgList)
	if !ok {
		return nil
	}
	params := make([]FunctionVar, 0, len(argList.Args))
	for _, argTypind := range argList.Args {
		params = append(params, FunctionVar{Type: r.resolve(argTypind)})
	}
	return params
}

func (r *typeResolver) loadFields(fieldListTypind uint16) []StructField {
	rec, ok := r.records[fieldListTypind]
	if !ok {
		return nil
	}
	fieldList, ok := rec.Parsed.(*nb09.LFFieldList)
	if !ok {
		return nil
	}
	fields := make([]StructField, 0, len(fieldList.Fields))
	for _, field := range fieldList.Fields {
		member, ok := field.Parsed.(*nb09.LFMember)
		if !ok {
			continue
		}
		typ := r.resolve(member.Type)
		bitfield := r.bitfield(member.Type)
		size := 0
		if bitfield != nil {
			typ = bitfield.BaseType
			size = bitfield.StorageSize
		} else if typ != nil {
			size = typ.Bytes()
		}
		offset := int(member.Offset.Value)
		fields = append(fields, StructField{
			Name:     member.Name,
			Type:     typ,
			Offset:   offset,
			Size:     size,
			End:      offset + size,
			Bitfield: bitfield,
		})
	}
	return fields
}

func (r *typeResolver) bitfield(typind uint16) *Bitfield {
	rec, ok := r.records[typind]
	if !ok {
		return nil
	}
	lf, ok := rec.Parsed.(*nb09.LFBitfield)
	if !ok {
		return nil
	}
	base := r.resolve(lf.BaseType)
	storageSize := 0
	if base != nil {
		storageSize = base.Bytes()
	}
	if storageSize <= 0 {
		storageSize = (int(lf.Position) + int(lf.Length) + 7) / 8
	}
	return &Bitfield{
		BaseType:    base,
		StorageSize: storageSize,
		BitOffset:   int(lf.Position),
		BitWidth:    int(lf.Length),
	}
}

func primitiveType(typind uint16) Type {
	hi := typind & 0xff00
	if hi == 0x0100 || hi == 0x0200 || hi == 0x0300 || hi == 0x0400 || hi == 0x0500 {
		base := primitiveType(typind & 0x00ff)
		return &Pointer{
			Elem:  base,
			Class: ptrClassFromSimpleMode(hi),
		}
	}

	base := typind & 0x00ff

	var typ Type
	switch base {
	case 0x0000, 0x0001, 0x0002:
		typ = &Primitive{TypeKind: KInvalid, Name: "notype"}
	case 0x0003:
		typ = &Primitive{TypeKind: KVoid, Name: "void"}
	case 0x0010:
		typ = &Primitive{TypeKind: KInt, Name: "int8_t", Size: 1, Signed: true}
	case 0x0011:
		typ = &Primitive{TypeKind: KInt, Name: "int16_t", Size: 2, Signed: true}
	case 0x0012:
		typ = &Primitive{TypeKind: KInt, Name: "int32_t", Size: 4, Signed: true}
	case 0x0020:
		typ = &Primitive{TypeKind: KInt, Name: "uint8_t", Size: 1}
	case 0x0021:
		typ = &Primitive{TypeKind: KInt, Name: "uint16_t", Size: 2}
	case 0x0022:
		typ = &Primitive{TypeKind: KInt, Name: "uint32_t", Size: 4}
	case 0x0040:
		typ = &Primitive{TypeKind: KFloat, Name: "float", Size: 4}
	case 0x0041:
		typ = &Primitive{TypeKind: KFloat, Name: "double", Size: 8}
	case 0x0070:
		typ = &Primitive{TypeKind: KInt, Name: "char", Size: 1, Signed: true}
	case 0x0071:
		typ = &Primitive{TypeKind: KInt, Name: "uint16_t", Size: 2}
	case 0x0072:
		typ = &Primitive{TypeKind: KInt, Name: "int16_t", Size: 2, Signed: true}
	case 0x0073:
		typ = &Primitive{TypeKind: KInt, Name: "uint16_t", Size: 2}
	case 0x0074:
		typ = &Primitive{TypeKind: KInt, Name: "int32_t", Size: 4, Signed: true}
	case 0x0075:
		typ = &Primitive{TypeKind: KInt, Name: "uint32_t", Size: 4}
	default:
		typ = unknownType(typind)
	}
	return typ
}

func unknownType(typind uint16) Type {
	return &Primitive{TypeKind: KInvalid, Name: fmt.Sprintf("typind_0x%04x", typind)}
}

func ptrClassFromAttr(attr uint32) PtrClass {
	switch attr & 0x001f {
	case 0x0001:
		return PtrFar
	case 0x0002:
		return PtrHuge
	default:
		return PtrNear
	}
}

func ptrClassFromSimpleMode(hi uint16) PtrClass {
	switch hi {
	case 0x0200, 0x0400, 0x0500:
		return PtrFar
	case 0x0300:
		return PtrHuge
	default:
		return PtrNear
	}
}

func arrayCount(length nb09.NumericLeaf, elem Type) int {
	if length.IsLeaf || length.Value <= 0 || elem == nil {
		return 0
	}
	elemBytes := elem.Bytes()
	if elemBytes <= 0 {
		return 0
	}
	return int(length.Value) / elemBytes
}

func callConvFromNB09(callType uint8) CallConv {
	switch callType {
	case 0x00, 0x01:
		return CCCdecl
	case 0x02, 0x03:
		return CCPascal
	case 0x07, 0x08:
		return CCStdcall
	default:
		return CCUnknown
	}
}
