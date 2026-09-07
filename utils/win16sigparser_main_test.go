package main

import "testing"

func TestParseParam_PreservesLPSTRAlias(t *testing.T) {
	got := parseParam("LPSTR lpszOut", 1)
	if got.Name != "lpszout" {
		t.Fatalf("name: got %q want %q", got.Name, "lpszout")
	}
	if got.CType != "LPSTR" {
		t.Fatalf("ctype: got %q want %q", got.CType, "LPSTR")
	}
}

func TestParseParam_ConstCharFarPointerUsesLPCSTR(t *testing.T) {
	got := parseParam("const char FAR *lpszFmt", 1)
	if got.Name != "lpszfmt" {
		t.Fatalf("name: got %q want %q", got.Name, "lpszfmt")
	}
	if got.CType != "LPCSTR" {
		t.Fatalf("ctype: got %q want %q", got.CType, "LPCSTR")
	}
}

func TestParseRetAndCallconv_LPSTR(t *testing.T) {
	ret, cc := parseRetAndCallconv("LPSTR WINAPI")
	if ret != "LPSTR" {
		t.Fatalf("ret: got %q want %q", ret, "LPSTR")
	}
	if cc != "pascal" {
		t.Fatalf("callconv: got %q want %q", cc, "pascal")
	}
}

func TestParseParam_UnnamedRectFarPointerPreserved(t *testing.T) {
	got := parseParam("RECT FAR*", 2)
	if got.Name != "arg2" {
		t.Fatalf("name: got %q want %q", got.Name, "arg2")
	}
	if got.CType != "RECT FAR*" {
		t.Fatalf("ctype: got %q want %q", got.CType, "RECT FAR*")
	}
}

func TestParseParam_UnnamedRectLowerFarPointerPreserved(t *testing.T) {
	got := parseParam("RECT far*", 2)
	if got.Name != "arg2" {
		t.Fatalf("name: got %q want %q", got.Name, "arg2")
	}
	if got.CType != "RECT FAR*" {
		t.Fatalf("ctype: got %q want %q", got.CType, "RECT FAR*")
	}
}
