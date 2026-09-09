package sem

// recoverLifetimes separates unrelated values that reuse the same physical
// bytes. Definitions are connected only when a read can observe them together.
// Escaped objects and undefined incoming storage retain their physical identity.
func (a *scratchAnalysis) recoverLifetimes() {
	parents := make([]int, len(a.defs))
	for id := range parents {
		parents[id] = id
	}
	var root func(int) int
	root = func(id int) int {
		if parents[id] != id {
			parents[id] = root(parents[id])
		}
		return parents[id]
	}
	for _, access := range a.accesses {
		if access.kind != scratchRead {
			continue
		}
		first := -1
		state := a.reads[access.point]
		o := a.object(access.region)
		for off := access.region.Offset; off < access.region.End(); off++ {
			if len(state[off]) == 0 || state[off][-1] {
				if o.disposition != scratchLocal {
					o.disposition = scratchUnresolved
				}
			}
			for id := range state[off] {
				if id < 0 {
					continue
				}
				if first < 0 {
					first = id
				} else {
					parents[root(id)] = root(first)
				}
			}
		}
	}
	physical := a.objects
	a.readObjects = map[scratchUseKey]*scratchObject{}
	groups := map[int]*scratchObject{}
	var objects []*scratchObject
	for _, o := range physical {
		if o.disposition == scratchLocal || o.disposition == scratchUnresolved {
			objects = append(objects, o)
		}
	}
	for id := range a.defs {
		def := &a.defs[id]
		o := a.object(def.region)
		if o.disposition == scratchLocal || o.disposition == scratchUnresolved {
			def.object = o
			continue
		}
		group := root(id)
		semantic, ok := groups[group]
		if !ok {
			semantic = &scratchObject{region: def.region}
			groups[group] = semantic
			objects = append(objects, semantic)
		} else {
			end := max(semantic.region.End(), def.region.End())
			semantic.region.Offset = min(semantic.region.Offset, def.region.Offset)
			semantic.region.Size = end - semantic.region.Offset
		}
		def.object = semantic
	}
	for _, access := range a.accesses {
		if access.kind == scratchWrite {
			continue
		}
		o := a.object(access.region)
		if o.disposition != scratchLocal && o.disposition != scratchUnresolved {
			for id := range a.reads[access.point][access.region.Offset] {
				o = a.defs[id].object
				break
			}
		}
		a.readObjects[scratchUseKey{access.point, access.region}] = o
	}
	a.objects = objects
	for _, access := range a.accesses {
		var o *scratchObject
		if access.kind == scratchWrite {
			o = a.defs[a.writes[access.point]].object
		} else {
			o = a.readObjects[scratchUseKey{access.point, access.region}]
		}
		if o.disposition == scratchLocal || o.disposition == scratchUnresolved {
			continue
		}
		o.observeType(access)
	}
	for _, o := range a.objects {
		if o.typ == nil {
			o.typ = scratchTypeForWidth(o.region.Size)
		}
	}
}
