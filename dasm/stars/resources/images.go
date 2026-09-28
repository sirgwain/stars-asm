package resources

import (
	"encoding/binary"
	"fmt"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
)

const (
	groupHeaderSize     = 6
	groupEntrySize      = 14
	fileEntrySize       = 16
	bitmapFileHeaderLen = 14
	cursorHotspotLen    = 4
	iconFileType        = 1
	cursorFileType      = 2
)

// iconFile assembles an .ico file from a GROUP_ICON resource and the ICON
// resources its directory entries name.
func iconFile(img *asm.ImageNE, group *asm.Resource) ([]byte, error) {
	entries, err := groupEntries(group)
	if err != nil {
		return nil, err
	}
	header := make([]byte, groupHeaderSize+fileEntrySize*len(entries))
	binary.LittleEndian.PutUint16(header[2:], iconFileType)
	binary.LittleEndian.PutUint16(header[4:], uint16(len(entries)))
	var images []byte
	for i, e := range entries {
		icon, ok := img.Resource(asm.ResourceIcon, asm.ResourceKey{Ordinal: true, ID: e.id})
		if !ok || int(e.bytes) > len(icon.Data) {
			return nil, fmt.Errorf("icon group %s: icon %d missing or short", group.Name, e.id)
		}
		entry := header[groupHeaderSize+fileEntrySize*i:]
		copy(entry[:8], e.dir[:])
		binary.LittleEndian.PutUint32(entry[8:], e.bytes)
		binary.LittleEndian.PutUint32(entry[12:], uint32(len(header)+len(images)))
		images = append(images, icon.Data[:e.bytes]...)
	}
	return append(header, images...), nil
}

// cursorFile assembles a .cur file from a GROUP_CURSOR resource and the
// CURSOR resources its directory entries name. Each CURSOR resource starts
// with its hotspot, which the file keeps in its directory entry instead.
func cursorFile(img *asm.ImageNE, group *asm.Resource) ([]byte, error) {
	entries, err := groupEntries(group)
	if err != nil {
		return nil, err
	}
	header := make([]byte, groupHeaderSize+fileEntrySize*len(entries))
	binary.LittleEndian.PutUint16(header[2:], cursorFileType)
	binary.LittleEndian.PutUint16(header[4:], uint16(len(entries)))
	var images []byte
	for i, e := range entries {
		cursor, ok := img.Resource(asm.ResourceCursor, asm.ResourceKey{Ordinal: true, ID: e.id})
		if !ok || int(e.bytes) > len(cursor.Data) || e.bytes < cursorHotspotLen {
			return nil, fmt.Errorf("cursor group %s: cursor %d missing or short", group.Name, e.id)
		}
		// The group entry holds the width and the doubled (XOR and AND
		// mask) height as words; the file entry holds byte dimensions of
		// one image and the hotspot.
		width := binary.LittleEndian.Uint16(e.dir[0:])
		height := binary.LittleEndian.Uint16(e.dir[2:]) / 2
		entry := header[groupHeaderSize+fileEntrySize*i:]
		entry[0], entry[1] = byte(width), byte(height)
		copy(entry[4:8], cursor.Data[:cursorHotspotLen])
		binary.LittleEndian.PutUint32(entry[8:], e.bytes-cursorHotspotLen)
		binary.LittleEndian.PutUint32(entry[12:], uint32(len(header)+len(images)))
		images = append(images, cursor.Data[cursorHotspotLen:e.bytes]...)
	}
	return append(header, images...), nil
}

// groupEntry is one directory entry of a GROUP_ICON or GROUP_CURSOR
// resource: its 8 dimension/format bytes, image size and image resource id.
type groupEntry struct {
	dir   [8]byte
	bytes uint32
	id    uint16
}

// groupEntries parses the directory of a GROUP_ICON or GROUP_CURSOR
// resource.
func groupEntries(group *asm.Resource) ([]groupEntry, error) {
	if len(group.Data) < groupHeaderSize {
		return nil, fmt.Errorf("group %s: truncated header", group.Name)
	}
	count := int(binary.LittleEndian.Uint16(group.Data[4:]))
	if len(group.Data) < groupHeaderSize+groupEntrySize*count {
		return nil, fmt.Errorf("group %s: truncated directory", group.Name)
	}
	entries := make([]groupEntry, count)
	for i := range entries {
		raw := group.Data[groupHeaderSize+groupEntrySize*i:]
		copy(entries[i].dir[:], raw[:8])
		entries[i].bytes = binary.LittleEndian.Uint32(raw[8:])
		entries[i].id = binary.LittleEndian.Uint16(raw[12:])
	}
	return entries, nil
}

// bitmapFile prefixes a BITMAP resource's packed DIB with the file header a
// .bmp file needs, trimming the resource's sector padding.
func bitmapFile(res *asm.Resource) ([]byte, error) {
	dib := res.Data
	if len(dib) < 4 {
		return nil, fmt.Errorf("bitmap %s: truncated header", res.Name)
	}
	headerSize := binary.LittleEndian.Uint32(dib)
	var width, height, bitCount, colors, imageSize, entrySize uint32
	switch headerSize {
	case 12: // BITMAPCOREHEADER
		width = uint32(binary.LittleEndian.Uint16(dib[4:]))
		height = uint32(binary.LittleEndian.Uint16(dib[6:]))
		bitCount = uint32(binary.LittleEndian.Uint16(dib[10:]))
		entrySize = 3
	case 40: // BITMAPINFOHEADER
		width = binary.LittleEndian.Uint32(dib[4:])
		height = binary.LittleEndian.Uint32(dib[8:])
		bitCount = uint32(binary.LittleEndian.Uint16(dib[14:]))
		imageSize = binary.LittleEndian.Uint32(dib[20:])
		colors = binary.LittleEndian.Uint32(dib[32:])
		entrySize = 4
	default:
		return nil, fmt.Errorf("bitmap %s: unsupported header size %d", res.Name, headerSize)
	}
	if colors == 0 && bitCount <= 8 {
		colors = 1 << bitCount
	}
	if imageSize == 0 {
		imageSize = (width*bitCount + 31) / 32 * 4 * height
	}
	bitsOffset := headerSize + colors*entrySize
	if total := bitsOffset + imageSize; total <= uint32(len(dib)) {
		dib = dib[:total]
	}
	out := make([]byte, bitmapFileHeaderLen, bitmapFileHeaderLen+len(dib))
	out[0], out[1] = 'B', 'M'
	binary.LittleEndian.PutUint32(out[2:], uint32(bitmapFileHeaderLen+len(dib)))
	binary.LittleEndian.PutUint32(out[10:], bitmapFileHeaderLen+bitsOffset)
	return append(out, dib...), nil
}
