package testfixture

import (
	"fmt"
	"path/filepath"
	"runtime"
	"sync"
	"testing"

	"github.com/sirgwain/stars-asm/dasm/starsenv"
)

type StarsFixture = starsenv.Env

var stars struct {
	once sync.Once
	fx   *StarsFixture
	err  error
}

func defaultInputDir() string {
	return filepath.Join(repoRoot(), "dasm", "input")
}

func repoRoot() string {
	_, file, _, _ := runtime.Caller(0)
	dir := filepath.Dir(file)
	for {
		if filepath.Base(dir) == "dasm" {
			return filepath.Dir(dir)
		}
		parent := filepath.Dir(dir)
		if parent == dir {
			panic("could not find repo root")
		}
		dir = parent
	}
}

func Stars(t testing.TB) *StarsFixture {
	t.Helper()

	stars.once.Do(func() {
		fx, err := starsenv.LoadStars(starsenv.Options{
			InputDir: defaultInputDir(),
		})
		if err != nil {
			stars.err = fmt.Errorf("starsenv.LoadDefault: %w", err)
			return
		}
		stars.fx = fx
	})

	if stars.err != nil {
		t.Fatalf("load Stars fixture: %v", stars.err)
	}
	return stars.fx
}
