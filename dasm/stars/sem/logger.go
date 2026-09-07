package sem

import (
	"context"
	"log/slog"
)

type semLogger struct {
	slog.Handler
	include func() bool
}

func (h *semLogger) Enabled(ctx context.Context, level slog.Level) bool {
	if h.include != nil && !h.include() {
		return false
	}
	return h.Handler.Enabled(ctx, level)
}

func (h *semLogger) Handle(ctx context.Context, r slog.Record) error {
	if h.include != nil && !h.include() {
		return nil
	}
	return h.Handler.Handle(ctx, r)
}

func (h *semLogger) WithAttrs(attrs []slog.Attr) slog.Handler {
	return &semLogger{
		Handler: h.Handler.WithAttrs(attrs),
		include: h.include,
	}
}

func (h *semLogger) WithGroup(name string) slog.Handler {
	return &semLogger{
		Handler: h.Handler.WithGroup(name),
		include: h.include,
	}
}
