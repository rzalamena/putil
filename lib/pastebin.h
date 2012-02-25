/*
 * Copyright (c) 2012 Rafael F. Zalamena <rzalamena@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef __PASTEBIN__
#define __PASTEBIN__

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN	256
#endif

#define PASTEBIN_MAXSIZE	524288	/* 0.5MB (http://pastebin.com/faq) */
#define PASTEBIN_HOST		"pastebin.com"

/* New API tips took from:
 * http://pastebin.com/api.php
 */
#define PASTEBIN_KEY_URL	"/api/api_post.php"

/* Old API stuff based on this program:
 * http://winappdbg.sourceforge.net/blog/pastebin.py
 */
#define PASTEBIN_URL		"/api_public.php"

enum pb_paste_duration {
	PB_FOREVER	= 0,	/* 'N'   */
	PB_TEN_MINUTES	= 1,	/* '10M' */
	PB_ONE_HOUR	= 2,	/* '1H'  */
	PB_ONE_DAY	= 3,	/* '1D'  */
	PB_ONE_MONTH	= 4,	/* '1M'  */

	PB_GET_DURATION /* Returns the current duration */
};

enum pb_privacy_setting {
	PB_PUBLIC	= 0,	/* '0' */
	PB_UNLISTED	= 1,	/* '1' */
	PB_PRIVATE	= 2,	/* '2' */

	PB_GET_PRIVACY /* Returns the current privacy setting */
};

/* new_pb_paste - new pastebin context
 *
 * This function returns an allocated paste context structure that uses
 * pastebin callbacks. Pastebin doesn't support server mode.
 *
 * On error returns NULL.
 */
struct paste_ctx	*new_pb_paste(void);

/* pb_paste_format_name - sets a paste_format by name
 * pb_paste_format_id - sets a paste_format by id
 *
 * Use these functions to set a paste format for syntax highlight.
 *
 * This is the currently supported list: (current default is 121 - Text)
 * ID  = Name                             = Real Name
 * =====================================================================
 *   1 = 4cs                              = 4CS
 *   2 = 6502acme                         = 6502 ACME Cross Assembler
 *   3 = 6502kickass                      = 6502 Kick Assembler
 *   4 = 6502tasm                         = 6502 TASM/64TASS
 *   5 = abap                             = ABAP
 *   6 = actionscript                     = ActionScript
 *   7 = actionscript3                    = ActionScript 3
 *   8 = ada                              = Ada
 *   9 = algol68                          = ALGOL 68
 *  10 = apache                           = Apache Log
 *  11 = applescript                      = AppleScript
 *  12 = apt_sources                      = APT Sources
 *  13 = asm                              = ASM (NASM)
 *  14 = asp                              = ASP
 *  15 = autoconf                         = autoconf
 *  16 = autohotkey                       = Autohotkey
 *  17 = autoit                           = AutoIt
 *  18 = avisynth                         = Avisynth
 *  19 = awk                              = Awk
 *  20 = bascomavr                        = BASCOM AVR
 *  21 = bash                             = Bash
 *  22 = basic4gl                         = Basic4GL
 *  23 = bibtex                           = BibTeX
 *  24 = blitzbasic                       = Blitz Basic
 *  25 = bnf                              = BNF
 *  26 = boo                              = BOO
 *  27 = bf                               = BrainFuck
 *  28 = c                                = C
 *  29 = c_mac                            = C for Macs
 *  30 = cil                              = C Intermediate Language
 *  31 = csharp                           = C#
 *  32 = cpp                              = C++
 *  33 = cpp-qt                           = C++ (with QT extensions)
 *  34 = c_loadrunner                     = C: Loadrunner
 *  35 = caddcl                           = CAD DCL
 *  36 = cadlisp                          = CAD Lisp
 *  37 = cfdg                             = CFDG
 *  38 = chaiscript                       = ChaiScript
 *  39 = clojure                          = Clojure
 *  40 = klonec                           = Clone C
 *  41 = klonecpp                         = Clone C++
 *  42 = cmake                            = CMake
 *  43 = cobol                            = COBOL
 *  44 = coffeescript                     = CoffeeScript
 *  45 = cfm                              = ColdFusion
 *  46 = css                              = CSS
 *  47 = cuesheet                         = Cuesheet
 *  48 = d                                = D
 *  49 = dcs                              = DCS
 *  50 = delphi                           = Delphi
 *  51 = oxygene                          = Delphi Prism (Oxygene)
 *  52 = diff                             = Diff
 *  53 = div                              = DIV
 *  54 = dos                              = DOS
 *  55 = dot                              = DOT
 *  56 = e                                = E
 *  57 = ecmascript                       = ECMAScript
 *  58 = eiffel                           = Eiffel
 *  59 = email                            = Email
 *  60 = epc                              = EPC
 *  61 = erlang                           = Erlang
 *  62 = fsharp                           = F#
 *  63 = falcon                           = Falcon
 *  64 = fo                               = FO Language
 *  65 = f1                               = Formula One
 *  66 = fortran                          = Fortran
 *  67 = freebasic                        = FreeBasic
 *  68 = freeswitch                       = FreeSWITCH
 *  69 = gambas                           = GAMBAS
 *  70 = gml                              = Game Maker
 *  71 = gdb                              = GDB
 *  72 = genero                           = Genero
 *  73 = genie                            = Genie
 *  74 = gettext                          = GetText
 *  75 = go                               = Go
 *  76 = groovy                           = Groovy
 *  77 = gwbasic                          = GwBasic
 *  78 = haskell                          = Haskell
 *  79 = hicest                           = HicEst
 *  80 = hq9plus                          = HQ9 Plus
 *  81 = html4strict                      = HTML
 *  82 = html5                            = HTML 5
 *  83 = icon                             = Icon
 *  84 = idl                              = IDL
 *  85 = ini                              = INI file
 *  86 = inno                             = Inno Script
 *  87 = intercal                         = INTERCAL
 *  88 = io                               = IO
 *  89 = j                                = J
 *  90 = java                             = Java
 *  91 = java5                            = Java 5
 *  92 = javascript                       = JavaScript
 *  93 = jquery                           = jQuery
 *  94 = kixtart                          = KiXtart
 *  95 = latex                            = Latex
 *  96 = lb                               = Liberty BASIC
 *  97 = lsl2                             = Linden Scripting
 *  98 = lisp                             = Lisp
 *  99 = llvm                             = LLVM
 * 100 = locobasic                        = Loco Basic
 * 101 = logtalk                          = Logtalk
 * 102 = lolcode                          = LOL Code
 * 103 = lotusformulas                    = Lotus Formulas
 * 104 = lotusscript                      = Lotus Script
 * 105 = lscript                          = LScript
 * 106 = lua                              = Lua
 * 107 = m68k                             = M68000 Assembler
 * 108 = magiksf                          = MagikSF
 * 109 = make                             = Make
 * 110 = mapbasic                         = MapBasic
 * 111 = matlab                           = MatLab
 * 112 = mirc                             = mIRC
 * 113 = mmix                             = MIX Assembler
 * 114 = modula2                          = Modula 2
 * 115 = modula3                          = Modula 3
 * 116 = 68000devpac                      = Motorola 68000 HiSoft Dev
 * 117 = mpasm                            = MPASM
 * 118 = mxml                             = MXML
 * 119 = mysql                            = MySQL
 * 120 = newlisp                          = newLISP
 * 121 = text                             = None
 * 122 = nsis                             = NullSoft Installer
 * 123 = oberon2                          = Oberon 2
 * 124 = objeck                           = Objeck Programming Langua
 * 125 = objc                             = Objective C
 * 126 = ocaml-brief                      = OCalm Brief
 * 127 = ocaml                            = OCaml
 * 128 = pf                               = OpenBSD PACKET FILTER
 * 129 = glsl                             = OpenGL Shading
 * 130 = oobas                            = Openoffice BASIC
 * 131 = oracle11                         = Oracle 11
 * 132 = oracle8                          = Oracle 8
 * 133 = oz                               = Oz
 * 134 = pascal                           = Pascal
 * 135 = pawn                             = PAWN
 * 136 = pcre                             = PCRE
 * 137 = per                              = Per
 * 138 = perl                             = Perl
 * 139 = perl6                            = Perl 6
 * 140 = php                              = PHP
 * 141 = php-brief                        = PHP Brief
 * 142 = pic16                            = Pic 16
 * 143 = pike                             = Pike
 * 144 = pixelbender                      = Pixel Bender
 * 145 = plsql                            = PL/SQL
 * 146 = postgresql                       = PostgreSQL
 * 147 = povray                           = POV-Ray
 * 148 = powershell                       = Power Shell
 * 149 = powerbuilder                     = PowerBuilder
 * 150 = proftpd                          = ProFTPd
 * 151 = progress                         = Progress
 * 152 = prolog                           = Prolog
 * 153 = properties                       = Properties
 * 154 = providex                         = ProvideX
 * 155 = purebasic                        = PureBasic
 * 156 = pycon                            = PyCon
 * 157 = python                           = Python
 * 158 = q                                = q/kdb+
 * 159 = qbasic                           = QBasic
 * 160 = rsplus                           = R
 * 161 = rails                            = Rails
 * 162 = rebol                            = REBOL
 * 163 = reg                              = REG
 * 164 = robots                           = Robots
 * 165 = rpmspec                          = RPM Spec
 * 166 = ruby                             = Ruby
 * 167 = gnuplot                          = Ruby Gnuplot
 * 168 = sas                              = SAS
 * 169 = scala                            = Scala
 * 170 = scheme                           = Scheme
 * 171 = scilab                           = Scilab
 * 172 = sdlbasic                         = SdlBasic
 * 173 = smalltalk                        = Smalltalk
 * 174 = smarty                           = Smarty
 * 175 = sql                              = SQL
 * 176 = systemverilog                    = SystemVerilog
 * 177 = tsql                             = T-SQL
 * 178 = tcl                              = TCL
 * 179 = teraterm                         = Tera Term
 * 180 = thinbasic                        = thinBasic
 * 181 = typoscript                       = TypoScript
 * 182 = unicon                           = Unicon
 * 183 = uscript                          = UnrealScript
 * 184 = vala                             = Vala
 * 185 = vbnet                            = VB.NET
 * 186 = verilog                          = VeriLog
 * 187 = vhdl                             = VHDL
 * 188 = vim                              = VIM
 * 189 = visualprolog                     = Visual Pro Log
 * 190 = vb                               = VisualBasic
 * 191 = visualfoxpro                     = VisualFoxPro
 * 192 = whitespace                       = WhiteSpace
 * 193 = whois                            = WHOIS
 * 194 = winbatch                         = Winbatch
 * 195 = xbasic                           = XBasic
 * 196 = xml                              = XML
 * 197 = xorg_conf                        = Xorg Config
 * 198 = xpp                              = XPP
 * 199 = yaml                             = YAML
 * 200 = z80                              = Z80 Assembler
 * 201 = zxbasic                          = ZXBasic
 *
 * If the name or key doesn't exists, it will be set the default choice
 * that is: 121 - text.
 */
void	 pb_paste_format_name(struct paste_ctx *, const char *);
void	 pb_paste_format_id(struct paste_ctx *, const int);

enum pb_paste_duration pb_duration(struct paste_ctx *,
    enum pb_paste_duration);
enum pb_privacy_setting pb_privacy(struct paste_ctx *,
    enum pb_privacy_setting);

#endif /* __PASTEBIN__ */
