package starsenv

import (
	"path/filepath"

	"github.com/sirgwain/stars-asm/dasm/stars/asm"
	"github.com/sirgwain/stars-asm/dasm/typeinfo"
)

type Env struct {
	SDB   *typeinfo.SymbolDB
	Image *asm.ImageNE	
}

type Options struct {
	InputDir string
}

func (o Options) ExePath() string {
	return filepath.Join(o.InputDir, "stars.exe")
}
