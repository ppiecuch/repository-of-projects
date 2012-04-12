/**
 * SyntaxHighlighter
 * http://alexgorbatchev.com/SyntaxHighlighter
 *
 * SyntaxHighlighter is donationware. If you are using it, please donate.
 * http://alexgorbatchev.com/SyntaxHighlighter/donate.html
 *
 * @version
 * 3.0.83 (July 02 2010)
 * 
 * @copyright
 * Copyright (C) 2004-2010 Alex Gorbatchev.
 *
 * @license
 * Dual licensed under the MIT and GPL licenses.
 */
;(function()
{
	// CommonJS
	typeof(require) != 'undefined' ? SyntaxHighlighter = require('shCore').SyntaxHighlighter : null;

	function Brush()
	{
		var keywords =
			'int ' +
			'GLuint ' +
			'NULL '
			;
		var functions =
			'glActiveTexture ' +
			'glBindBuffer ' +
			'glBufferData ' +
			'glBufferSubData ' +
			'glCompressedTexImage2D ' +
			'glCompressedTexSubImage2D ' +
			'glDeleteBuffers ' +
			'glDisable ' +
			'glDrawElements ' +
			'glEnable ' +
			'glGenBuffers ' +
			'glLoadMatrix ' +
			'glMatrixMode ' +
			'glNormalPointer ' +
			'glPixelStorei ' +
			'glReadPixels ' +
			'glScissor ' +
			'glTexImage2D ' +
			'glTexSubImage2D '
			;
		var constants =	
			'GL_ARRAY_BUFFER ' +
			'GL_ARRAY_BUFFER_BINDING ' +
			'GL_DST_ALPHA ' +
			'GL_DST_COLOR ' +
			'GL_DYNAMIC_DRAW ' +
			'GL_ELEMENT_ARRAY_BUFFER ' +
			'GL_INVALID_OPERATION ' +
			'GL_MAX_TEXTURE_UNITS ' +
			'GL_NORMAL_ARRAY_BUFFER_BINDING ' +
			'GL_ONE ' +
			'GL_ONE_MINUS_DST_ALPHA ' +
			'GL_ONE_MINUS_DST_COLOR ' +
			'GL_ONE_MINUS_SRC_ALPHA ' +
			'GL_ONE_MINUS_SRC_COLOR ' +
			'GL_PACK_ALIGNMENT ' +
			'GL_READ_WRITE ' +
			'GL_SCISSOR_TEST ' +
			'GL_SRC_ALPHA ' +
			'GL_SRC_ALPHA_SATURATE ' +
			'GL_SRC_COLOR ' +
			'GL_STATIC_DRAW ' +
			'GL_STREAM_DRAW ' +
			'GL_TEXTURE0 ' +
			'GL_TEXTUREi ' +
			'GL_UNPACK_ALIGNMENT ' +
			'GL_ZERO' 
			;

		var r = SyntaxHighlighter.regexLib;
		
		this.regexList = [
			//{ regex: r.multiLineDoubleQuotedString,					css: 'string' },			// double quoted strings
			//{ regex: r.multiLineSingleQuotedString,					css: 'string' },			// single quoted strings
			{ regex: r.singleLineCComments,							css: 'comments' },			// one line comments
			{ regex: r.multiLineCComments,							css: 'comments' },			// multiline comments
			{ regex: /\s*#.*/gm,									css: 'preprocessor' },		// preprocessor tags like #region and #endregion
			{ regex: /\u2605.*/gm,									css: 'green' },				// preprocessor tags like #region and #endregion
			{ regex: /\u3010.*\u3011/gm,							css: 'h3' },				// h3
			{ regex: new RegExp(this.getKeywords(keywords), 'gm'),	css: 'keyword' },			// keywords
			{ regex: new RegExp(this.getKeywords(constants), 'gm'),	css: 'constants' },			// constants
			{ regex: new RegExp(this.getKeywords(functions), 'gm'),	css: 'functions' }			// functions
			];
	
		this.forHtmlScript(r.scriptScriptTags);
	};

	Brush.prototype	= new SyntaxHighlighter.Highlighter();
	Brush.aliases	= ['gl', 'ogl', 'opengl'];

	SyntaxHighlighter.brushes.JScript = Brush;

	// CommonJS
	typeof(exports) != 'undefined' ? exports.Brush = Brush : null;
})();
