// Package savefile reads Stars! game files (.xy, .hst, .mN, .xN, .hN) into
// decrypted records, following the original ReadRt/SetFileXorStream code.
package savefile

import (
	"encoding/binary"
	"fmt"
)

// Record types used by the dumper. The full list lives in the RecordType enum
// in dasm/input/enums.h.
const (
	RtEOF     = 0
	RtBOF     = 8
	RtMsg     = 12
	RtMsgFilt = 33
	RtPlrMsg  = 40
)

// Tables holds the static data from stars.exe needed to decode a file.
type Tables struct {
	// Primes is rgPrimes, the seed table for the file XOR stream.
	Primes []int16
	// MsgArgCounts is rgcMsgArgs, the number of parameters for each MessageId.
	MsgArgCounts []byte
}

// Record is one decrypted file record.
type Record struct {
	Offset int
	Type   int
	Data   []byte
}

// BOF is the decoded file header record (RTBOF).
type BOF struct {
	Magic     string
	LidGame   int32
	VerMajor  int
	VerMinor  int
	VerInc    int
	Turn      uint16
	IPlayer   int16
	LSaltTime int16
	Dt        int
	FDone     bool
	FInUse    bool
	FMulti    bool
	FGameOver bool
	FCrippled bool
	WGen      int
}

// ParseBOF decodes an rtBOF record payload.
func ParseBOF(data []byte) (BOF, error) {
	if len(data) < 16 {
		return BOF{}, fmt.Errorf("BOF record is %d bytes, want 16", len(data))
	}
	ver := binary.LittleEndian.Uint16(data[8:])
	w12 := binary.LittleEndian.Uint16(data[12:])
	w14 := binary.LittleEndian.Uint16(data[14:])
	return BOF{
		Magic:     string(data[0:4]),
		LidGame:   int32(binary.LittleEndian.Uint32(data[4:])),
		VerInc:    int(ver & 0x1f),
		VerMinor:  int((ver >> 5) & 0x7f),
		VerMajor:  int(ver >> 12),
		Turn:      binary.LittleEndian.Uint16(data[10:]),
		IPlayer:   int16(w12<<11) >> 11,
		LSaltTime: int16(w12) >> 5,
		Dt:        int(w14 & 0xff),
		FDone:     w14&0x100 != 0,
		FInUse:    w14&0x200 != 0,
		FMulti:    w14&0x400 != 0,
		FGameOver: w14&0x800 != 0,
		FCrippled: w14&0x1000 != 0,
		WGen:      int(w14 >> 13),
	}, nil
}

// xorStream is the file XOR generator (lFileSeed1/lFileSeed2).
type xorStream struct {
	s1, s2 int32
}

// newXorStream seeds the XOR stream from a file header, as SetFileXorStream.
func newXorStream(primes []int16, bof BOF) (*xorStream, error) {
	salt := bof.LSaltTime
	a := int(salt & 0x1f)
	b := int((salt >> 5) & 0x1f)
	if salt&0x400 != 0 {
		a += 32
	} else {
		b += 32
	}
	if a >= len(primes) || b >= len(primes) {
		return nil, fmt.Errorf("prime index %d/%d out of range of %d primes", a, b, len(primes))
	}
	x := &xorStream{s1: int32(primes[a]), s2: int32(primes[b])}
	crippled := int16(0)
	if bof.FCrippled {
		crippled = 1
	}
	n := ((int16(bof.LidGame)&3)+1)*((int16(bof.Turn)&3)+1)*((bof.IPlayer&3)+1) + crippled
	for ; n > 0; n-- {
		x.next()
	}
	return x, nil
}

// next advances the generator, as LGetNextFileXor.
func (x *xorStream) next() int32 {
	s1, s2 := x.s1, x.s2
	k := s1 / 53668
	s1 = (s1-k*53668)*40014 - k*12211
	if s1 < 0 {
		s1 += 2147483563
	}
	k = s2 / 52774
	s2 = (s2-k*52774)*40692 - k*3791
	if s2 < 0 {
		s2 += 2147483399
	}
	x.s1, x.s2 = s1, s2
	return s1 - s2
}

// xor decrypts buf in place, as XorFileBuf.
func (x *xorStream) xor(buf []byte) {
	i := 0
	for ; i+4 <= len(buf); i += 4 {
		v := binary.LittleEndian.Uint32(buf[i:]) ^ uint32(x.next())
		binary.LittleEndian.PutUint32(buf[i:], v)
	}
	if i < len(buf) {
		l := uint32(x.next())
		for ; i < len(buf); i++ {
			buf[i] ^= byte(l)
			l >>= 8
		}
	}
}

// ReadRecords splits a file into records and decrypts each one. Every rtBOF
// record reseeds the XOR stream; records before the first rtBOF are an error.
func ReadRecords(b []byte, t Tables) ([]Record, error) {
	var recs []Record
	var x *xorStream
	for off := 0; off < len(b); {
		if off+2 > len(b) {
			return recs, fmt.Errorf("truncated record header at 0x%x", off)
		}
		hdr := binary.LittleEndian.Uint16(b[off:])
		rt, cb := int(hdr>>10), int(hdr&0x3ff)
		if off+2+cb > len(b) {
			return recs, fmt.Errorf("record rt=%d cb=%d at 0x%x runs past end of file", rt, cb, off)
		}
		data := append([]byte(nil), b[off+2:off+2+cb]...)
		switch {
		case rt == RtBOF:
			bof, err := ParseBOF(data)
			if err != nil {
				return recs, fmt.Errorf("at 0x%x: %w", off, err)
			}
			if x, err = newXorStream(t.Primes, bof); err != nil {
				return recs, fmt.Errorf("at 0x%x: %w", off, err)
			}
		case rt == RtEOF:
		case x == nil:
			return recs, fmt.Errorf("record rt=%d at 0x%x precedes the file header", rt, off)
		default:
			x.xor(data)
		}
		recs = append(recs, Record{Offset: off, Type: rt, Data: data})
		off += 2 + cb
	}
	return recs, nil
}

// Message is one turn message from an rtMsg record (MSGHDR plus parameters).
type Message struct {
	ID   int
	Goto int16
	// Wide marks parameters stored as 16 bits (grWord); others are one byte.
	Wide []bool
	Args []uint16
}

// ParseMessages decodes the packed messages in an rtMsg record, walking them
// as ReadPlayerMessages does.
func ParseMessages(data []byte, argCounts []byte) ([]Message, error) {
	var msgs []Message
	for off := 0; off < len(data); {
		if off+4 > len(data) {
			return msgs, fmt.Errorf("truncated message header at +0x%x", off)
		}
		w := binary.LittleEndian.Uint16(data[off:])
		m := Message{ID: int(w & 0x1ff), Goto: int16(binary.LittleEndian.Uint16(data[off+2:]))}
		if m.ID >= len(argCounts) {
			return msgs, fmt.Errorf("message id %d at +0x%x is past %d known messages", m.ID, off, len(argCounts))
		}
		off += 4
		gr := w >> 9
		for i := 0; i < int(argCounts[m.ID]); i++ {
			wide := gr&1 != 0
			gr >>= 1
			n := 1
			if wide {
				n = 2
			}
			if off+n > len(data) {
				return msgs, fmt.Errorf("message id %d argument %d runs past record end", m.ID, i)
			}
			v := uint16(data[off])
			if wide {
				v = binary.LittleEndian.Uint16(data[off:])
			}
			m.Wide = append(m.Wide, wide)
			m.Args = append(m.Args, v)
			off += n
		}
		msgs = append(msgs, m)
	}
	return msgs, nil
}

// PlayerMessage is a player-to-player message from an rtPlrMsg record (MSGPLR
// without its leading 4-byte list pointer).
type PlayerMessage struct {
	IPlrFrom int16
	IPlrTo   int16
	IInRe    int16
	CLen     int16
	Text     []byte
}

// ParsePlayerMessage decodes an rtPlrMsg record.
func ParsePlayerMessage(data []byte) (PlayerMessage, error) {
	if len(data) < 12 {
		return PlayerMessage{}, fmt.Errorf("player message record is %d bytes, want at least 12", len(data))
	}
	return PlayerMessage{
		IPlrFrom: int16(binary.LittleEndian.Uint16(data[4:])),
		IPlrTo:   int16(binary.LittleEndian.Uint16(data[6:])),
		IInRe:    int16(binary.LittleEndian.Uint16(data[8:])),
		CLen:     int16(binary.LittleEndian.Uint16(data[10:])),
		Text:     data[12:],
	}, nil
}
