#pragma once

typedef
enum FontWeight
{
    /// <summary>
    /// Predefined font weight : Thin (100).
    /// </summary>
    FONT_WEIGHT_THIN = 100,

    /// <summary>
    /// Predefined font weight : Extra-light (200).
    /// </summary>
    FONT_WEIGHT_EXTRA_LIGHT = 200,

    /// <summary>
    /// Predefined font weight : Ultra-light (200).
    /// </summary>
    FONT_WEIGHT_ULTRA_LIGHT = 200,

    /// <summary>
    /// Predefined font weight : Light (300).
    /// </summary>
    FONT_WEIGHT_LIGHT = 300,

    /// <summary>
    /// Predefined font weight : Semi-light (350).
    /// </summary>
    FONT_WEIGHT_SEMI_LIGHT = 350,

    /// <summary>
    /// Predefined font weight : Normal (400).
    /// </summary>
    FONT_WEIGHT_NORMAL = 400,

    /// <summary>
    /// Predefined font weight : Regular (400).
    /// </summary>
    FONT_WEIGHT_REGULAR = 400,

    /// <summary>
    /// Predefined font weight : Medium (500).
    /// </summary>
    FONT_WEIGHT_MEDIUM = 500,

    /// <summary>
    /// Predefined font weight : Demi-bold (600).
    /// </summary>
    FONT_WEIGHT_DEMI_BOLD = 600,

    /// <summary>
    /// Predefined font weight : Semi-bold (600).
    /// </summary>
    FONT_WEIGHT_SEMI_BOLD = 600,

    /// <summary>
    /// Predefined font weight : Bold (700).
    /// </summary>
    FONT_WEIGHT_BOLD = 700,

    /// <summary>
    /// Predefined font weight : Extra-bold (800).
    /// </summary>
    FONT_WEIGHT_EXTRA_BOLD = 800,

    /// <summary>
    /// Predefined font weight : Ultra-bold (800).
    /// </summary>
    FONT_WEIGHT_ULTRA_BOLD = 800,

    /// <summary>
    /// Predefined font weight : Black (900).
    /// </summary>
    FONT_WEIGHT_BLACK = 900,

    /// <summary>
    /// Predefined font weight : Heavy (900).
    /// </summary>
    FONT_WEIGHT_HEAVY = 900,

    /// <summary>
    /// Predefined font weight : Extra-black (950).
    /// </summary>
    FONT_WEIGHT_EXTRA_BLACK = 950,

    /// <summary>
    /// Predefined font weight : Ultra-black (950).
    /// </summary>
    FONT_WEIGHT_ULTRA_BLACK = 950
}   FontWeight;

typedef
enum FontStyle
{
    /// <summary>
    /// Font slope style : Normal.
    /// </summary>
    FONT_STYLE_NORMAL,

    /// <summary>
    /// Font slope style : Oblique.
    /// </summary>
    FONT_STYLE_OBLIQUE,

    /// <summary>
    /// Font slope style : Italic.
    /// </summary>
    FONT_STYLE_ITALIC

}   FontStyle;

typedef
enum FontStretch
{
    /// <summary>
    /// Predefined font stretch : Not known (0).
    /// </summary>
    FONT_STRETCH_UNDEFINED = 0,

    /// <summary>
    /// Predefined font stretch : Ultra-condensed (1).
    /// </summary>
    FONT_STRETCH_ULTRA_CONDENSED = 1,

    /// <summary>
    /// Predefined font stretch : Extra-condensed (2).
    /// </summary>
    FONT_STRETCH_EXTRA_CONDENSED = 2,

    /// <summary>
    /// Predefined font stretch : Condensed (3).
    /// </summary>
    FONT_STRETCH_CONDENSED = 3,

    /// <summary>
    /// Predefined font stretch : Semi-condensed (4).
    /// </summary>
    FONT_STRETCH_SEMI_CONDENSED = 4,

    /// <summary>
    /// Predefined font stretch : Normal (5).
    /// </summary>
    FONT_STRETCH_NORMAL = 5,

    /// <summary>
    /// Predefined font stretch : Medium (5).
    /// </summary>
    FONT_STRETCH_MEDIUM = 5,

    /// <summary>
    /// Predefined font stretch : Semi-expanded (6).
    /// </summary>
    FONT_STRETCH_SEMI_EXPANDED = 6,

    /// <summary>
    /// Predefined font stretch : Expanded (7).
    /// </summary>
    FONT_STRETCH_EXPANDED = 7,

    /// <summary>
    /// Predefined font stretch : Extra-expanded (8).
    /// </summary>
    FONT_STRETCH_EXTRA_EXPANDED = 8,

    /// <summary>
    /// Predefined font stretch : Ultra-expanded (9).
    /// </summary>
    FONT_STRETCH_ULTRA_EXPANDED = 9
}   FontStretch;

typedef
enum TextAlignment
{
    /// <summary>
    /// The leading edge of the paragraph text is aligned to the layout box's leading edge.
    /// </summary>
    TEXT_ALIGNMENT_LEADING,

    /// <summary>
    /// The trailing edge of the paragraph text is aligned to the layout box's trailing edge.
    /// </summary>
    TEXT_ALIGNMENT_TRAILING,

    /// <summary>
    /// The center of the paragraph text is aligned to the center of the layout box.
    /// </summary>
    TEXT_ALIGNMENT_CENTER,

    /// <summary>
    /// Align text to the leading side, and also justify text to fill the lines.
    /// </summary>
    TEXT_ALIGNMENT_JUSTIFIED
}   TextAlignment;

typedef
enum ParagraphAlignment
{
    /// <summary>
    /// The first line of paragraph is aligned to the flow's beginning edge of the layout box.
    /// </summary>
    PARAGRAPH_ALIGNMENT_NEAR,

    /// <summary>
    /// The last line of paragraph is aligned to the flow's ending edge of the layout box.
    /// </summary>
    PARAGRAPH_ALIGNMENT_FAR,

    /// <summary>
    /// The center of the paragraph is aligned to the center of the flow of the layout box.
    /// </summary>
    PARAGRAPH_ALIGNMENT_CENTER
}   ParagraphAlignment;

typedef
enum LineSpacingMethod
{
    /// <summary>
    /// Line spacing depends solely on the content, growing to accommodate the size of fonts and inline objects.
    /// </summary>
    LINE_SPACING_METHOD_DEFAULT,

    /// <summary>
    /// Lines are explicitly set to uniform spacing, regardless of contained font sizes.
    /// This can be useful to avoid the uneven appearance that can occur from font fallback.
    /// </summary>
    LINE_SPACING_METHOD_UNIFORM,

    /// <summary>
    /// Line spacing and baseline distances are proportional to the computed values based on the content, the size of the fonts and inline objects.
    /// </summary>
    LINE_SPACING_METHOD_PROPORTIONAL
}   LineSpacingMethod;

struct Rect
{
    float left;
    float top;
    float right;
    float bottom;
};
