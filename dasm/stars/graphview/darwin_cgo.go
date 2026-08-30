//go:build darwin && production

package graphview

/*
#cgo darwin LDFLAGS: -framework UniformTypeIdentifiers
*/
import "C"
