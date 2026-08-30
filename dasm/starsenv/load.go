package starsenv

import (
	"fmt"
	"path/filepath"

	"github.com/sirgwain/stars-asm/dasm/nb09"
	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

func LoadStars(opt Options) (*Env, error) {
	exePath := opt.ExePath()

	// load the nb09 debug data from the binary
	db, err := nb09.Load(exePath)
	if err != nil {
		return nil, fmt.Errorf("load nb09 %s: %w", exePath, err)
	}
	sdb, err := typeinfo.Load(opt.InputDir, db)
	if err != nil {
		return nil, fmt.Errorf("load symboldb %s: %w", exePath, err)
	}

	// open the NE part of the binary for call fixups, seg:off mapping, etc
	img, err := asm.OpenNE(exePath)
	if err != nil {
		return nil, fmt.Errorf("open exe/ne %s: %w", exePath, err)
	}

	exportFiles, err := filepath.Glob(filepath.Join(opt.InputDir, "*.exports"))
	if err != nil {
		return nil, fmt.Errorf("find exports in %s: %w", opt.InputDir, err)
	}
	if err := img.LoadExports(exportFiles...); err != nil {
		return nil, fmt.Errorf("load exports: %w", err)
	}

	return &Env{
		SDB:   sdb,
		Image: img,
	}, nil
}
