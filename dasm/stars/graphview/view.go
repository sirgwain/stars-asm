package graphview

import (
	"bytes"
	"embed"
	"encoding/json"
	"fmt"
	"html/template"
	"testing/fstest"

	"github.com/sirgwain/stars-asm/dasm/stars"
	"github.com/wailsapp/wails/v2"
	"github.com/wailsapp/wails/v2/pkg/options"
	"github.com/wailsapp/wails/v2/pkg/options/assetserver"
)

//go:embed assets/cytoscape.min.js assets/dagre.min.js assets/cytoscape-dagre.js assets/index.html.templ
var assets embed.FS

func Run(data *stars.FuncGraphViewData) error {
	html, err := renderIndex(data)
	if err != nil {
		return err
	}
	cytoscapeJS, err := assets.ReadFile("assets/cytoscape.min.js")
	if err != nil {
		return err
	}
	dagreJS, err := assets.ReadFile("assets/dagre.min.js")
	if err != nil {
		return err
	}
	cytoscapeDagreJS, err := assets.ReadFile("assets/cytoscape-dagre.js")
	if err != nil {
		return err
	}

	assetFS := fstest.MapFS{
		"index.html": {
			Data: []byte(html),
		},
		"cytoscape.min.js": {
			Data: cytoscapeJS,
		},
		"dagre.min.js": {
			Data: dagreJS,
		},
		"cytoscape-dagre.js": {
			Data: cytoscapeDagreJS,
		},
	}

	return wails.Run(&options.App{
		Title:     fmt.Sprintf("CFG graph: %s", data.Function),
		Width:     1440,
		Height:    980,
		MinWidth:  900,
		MinHeight: 640,
		AssetServer: &assetserver.Options{
			Assets: assetFS,
		},
		BackgroundColour: options.NewRGB(22, 24, 28),
	})
}

func renderIndex(data *stars.FuncGraphViewData) (string, error) {
	payload, err := json.Marshal(data)
	if err != nil {
		return "", err
	}

	tmpl, err := template.ParseFS(assets, "assets/index.html.templ")
	if err != nil {
		return "", err
	}

	var html bytes.Buffer
	if err := tmpl.Execute(&html, struct {
		GraphData template.JS
	}{
		GraphData: template.JS(payload),
	}); err != nil {
		return "", err
	}
	return html.String(), nil
}
