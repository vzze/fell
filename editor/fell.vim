" Vim syntax file
" Language: Fell

if exists("b:current_syntax")
    finish
endif

syn keyword fellConditionals if else
syn keyword fellRepeats for in do while
syn keyword fellKeywords ret assert nihil
syn keyword fellBoolean false true

syn match fellOperators "[<>=!*/%+-]"

syn match fellNumber "\<\d\+\>"
syn match fellNumber "\.\d\+\>"
syn match fellNumber "\<\d\+\.\d*\>"

syn match fellEscaped display contained "\\[fnrtv\\\"]"

syn region fellCommentLine start="#" end="$"
syn region fellFunctionDef start="|" end="|"
syn region fellString start="\"" end="\"" contains=fellEscaped

syn match fellIdentifier "\<\I\+\>"
syn match fellIdentifier "\<\I\i\+\>"

hi def link fellConditionals Conditional
hi def link fellRepeats Repeat
hi def link fellKeywords Keyword
hi def link fellBoolean Boolean

hi def link fellOperators Operator
hi def link fellNumber Number

hi def link fellString String
hi def link fellCommentLine Comment

hi def link fellFunctionDef Identifier
hi def link fellIdentifier Identifier

hi def link fellEscaped SpecialChar

let b:current_syn = "fell"
