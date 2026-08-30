package machine

import (
	"context"
	"log/slog"
)

type extractorLogger struct {
	slog.Handler
	include func() bool
}

func (h *extractorLogger) Enabled(ctx context.Context, level slog.Level) bool {
	if h.include != nil && !h.include() {
		return false
	}
	return h.Handler.Enabled(ctx, level)
}

func (h *extractorLogger) Handle(ctx context.Context, r slog.Record) error {
	if h.include != nil && !h.include() {
		return nil
	}
	return h.Handler.Handle(ctx, r)
}

func (h *extractorLogger) WithAttrs(attrs []slog.Attr) slog.Handler {
	return &extractorLogger{
		Handler: h.Handler.WithAttrs(attrs),
		include: h.include,
	}
}

func (h *extractorLogger) WithGroup(name string) slog.Handler {
	return &extractorLogger{
		Handler: h.Handler.WithGroup(name),
		include: h.include,
	}
}
