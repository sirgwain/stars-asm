package stars

import (
	"encoding/binary"
	"log/slog"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

// decodePointerInitializer decodes a near or far pointer from buf.
func decodePointerInitializer(
	img *asm.ImageNE,
	sdb *typeinfo.SymbolDB,
	loc initLocation,
	typ *typeinfo.Pointer,
	buf []byte,
) (*Initializer, bool) {
	if typ.Bytes() > len(buf) {
		return nil, false
	}
	if typ.Class == typeinfo.PtrFar || typ.Class == typeinfo.PtrHuge {
		return decodeFarPointerInitializer(img, sdb, loc, typ, buf)
	}
	return decodeNearPointerInitializer(img, sdb, loc, typ, buf)
}

func decodeNearPointerInitializer(
	img *asm.ImageNE,
	sdb *typeinfo.SymbolDB,
	loc initLocation,
	typ *typeinfo.Pointer,
	buf []byte,
) (*Initializer, bool) {
	ptrOff := uint32(binary.LittleEndian.Uint16(buf[:2]))

	// Null pointer.
	if ptrOff == 0 {
		return &Initializer{Kind: InitAddress, Type: typ, Addr: &InitAddrTarget{}}, true
	}

	// Near static pointers are relative to the owning global's NE segment.
	if typ.IsCStringPointer() {
		if s, ok := img.ReadCStringAt(loc.Seg, ptrOff); ok {
			return &Initializer{Kind: InitString, Type: typ, String: s}, true
		}
	}

	addr, ok := resolveStaticPointerTarget(sdb, loc.Seg, ptrOff)
	if !ok {
		addr = &InitAddrTarget{RawSeg: loc.Seg, RawOff: ptrOff}
	}
	return &Initializer{Kind: InitAddress, Type: typ, Addr: addr}, true
}

func decodeFarPointerInitializer(
	img *asm.ImageNE,
	sdb *typeinfo.SymbolDB,
	log initLocation,
	typ *typeinfo.Pointer,
	buf []byte,
) (*Initializer, bool) {
	ptrOff := uint32(binary.LittleEndian.Uint16(buf[:2]))
	ptrSeg := uint16(binary.LittleEndian.Uint16(buf[2:4]))

	// Null pointer.
	if ptrOff == 0 && ptrSeg == 0 {
		return &Initializer{Kind: InitAddress, Type: typ, Addr: &InitAddrTarget{}}, true
	}

	targetSeg := ptrSeg
	// The segment half of a far pointer starts two bytes into it.
	if fx, ok := img.FixupAt(int(log.Seg), uint16(log.Off+2)); ok {
		if fx.Source == asm.FixupSourceSegment &&
			fx.Target == asm.FixupTargetInternalRef {
			targetSeg = fx.TargetSegNum
		}
	}

	// Function pointer.
	if typeinfo.IsFunctionPointer(typ) {
		if fn := sdb.GetFunctionByAddr(typeinfo.Addr{
			Seg: targetSeg,
			Off: ptrOff,
		}); fn != nil {
			return &Initializer{
				Kind: InitAddress,
				Type: typ,
				Addr: &InitAddrTarget{
					Function:  fn,
					RawSeg:    targetSeg,
					RawOff:    ptrOff,
					ExactBase: true,
				},
			}, true
		} else {
			slog.Warn("no func pointer")
		}
	}

	if typ.IsCStringPointer() {
		if s, ok := img.ReadCStringAt(targetSeg, ptrOff); ok {
			return &Initializer{Kind: InitString, Type: typ, String: s}, true
		}
	}

	addr, ok := resolveStaticPointerTarget(sdb, targetSeg, ptrOff)
	if !ok {
		addr = &InitAddrTarget{RawSeg: targetSeg, RawOff: ptrOff}
	}
	return &Initializer{Kind: InitAddress, Type: typ, Addr: addr}, true
}

// resolveStaticPointerTarget tries to resolve an NE seg:off to a named global.
func resolveStaticPointerTarget(sdb *typeinfo.SymbolDB, seg uint16, off uint32) (*InitAddrTarget, bool) {
	g, byteOff, ok := sdb.GetGlobalContaining(typeinfo.Addr{Seg: seg, Off: off})
	if !ok || g == nil {
		return nil, false
	}
	return &InitAddrTarget{
		Symbol:    g,
		ByteOff:   byteOff,
		RawSeg:    seg,
		RawOff:    off,
		ExactBase: byteOff == 0,
	}, true
}
