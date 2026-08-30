package log

import (
	"context"
	"log/slog"
	"slices"
)

// LazyHandler wraps a Handler and defers resolution of LogValuer attributes
// until Handle time instead of WithAttrs time.
type LazyHandler struct {
	slog.Handler
	lazy []slog.Attr
}

func (h *LazyHandler) WithAttrs(attrs []slog.Attr) slog.Handler {
	var pass, hold []slog.Attr
	for _, a := range attrs {
		if _, ok := a.Value.Any().(slog.LogValuer); ok {
			hold = append(hold, a)
		} else {
			pass = append(pass, a)
		}
	}
	next := h.Handler
	if len(pass) > 0 {
		next = next.WithAttrs(pass)
	}
	return &LazyHandler{
		Handler: next,
		lazy:    append(slices.Clone(h.lazy), hold...),
	}
}

func (h *LazyHandler) WithGroup(name string) slog.Handler {
	return &LazyHandler{Handler: h.Handler.WithGroup(name), lazy: h.lazy}
}

func (h *LazyHandler) Handle(ctx context.Context, r slog.Record) error {
	if len(h.lazy) == 0 {
		return h.Handler.Handle(ctx, r)
	}
	r2 := slog.NewRecord(r.Time, r.Level, r.Message, r.PC)
	r2.AddAttrs(h.lazy...)
	r.Attrs(func(a slog.Attr) bool { r2.AddAttrs(a); return true })
	return h.Handler.Handle(ctx, r2)
}
