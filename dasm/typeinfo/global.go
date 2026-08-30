package typeinfo

type Var interface {
	variable()
	VarType() Type
}

type GlobalVar struct {
	Name       string
	Type       Type
	Addr       Addr
	Module     string
	RefModules []string

	// loaded outside symboldb
	StaticInitializer string
}

func (g *GlobalVar) CDecl() string {
	return TypeDecl(g.Type, g.Name)
}

func (g *GlobalVar) variable()     {}
func (g *GlobalVar) VarType() Type { return g.Type }
