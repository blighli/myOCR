/*
 *    File:          MobilOCR.h
 *
 *    Contents:      ABBYY Mobile OCR Engine API declarations.
 *
 *    Copyright  (c) 2012 ABBYY. All rights reserved.
 */

#ifndef MobilOCR_h
#define MobilOCR_h

#define MOBILE_OCR_LABEL_TEXT L"Powered by ABBYY"

/* ********************************************************************************************* */

#if defined( _WIN32 ) && !defined( __GNUC__ )
    #include <windows.h>
#elif defined( BUILD_MOBILEOCR )
    #include <WindowsTypes.h>
#else
    /* Windows type definitions for non-Windows platforms */

#ifndef FREMBED_WIN_TYPES
#define FREMBED_WIN_TYPES
    /* Fixed-size unsigned */
    typedef unsigned char BYTE;  /* 1-byte */
    typedef unsigned short WORD; /* 2-byte */
    typedef unsigned long DWORD; /* 4-byte */

    /* Unicode character */
    typedef WORD WCHAR;

    /* Simple rectangle */
    typedef struct tagRECT {
        long left;
        long top;
        long right;
        long bottom;
    } RECT, *PRECT;
#endif /* !FREMBED_WIN_TYPES */

#endif

/* MOBILEOCR_STATIC and FREMBED_DLL default definition */
#if !defined( MOBILEOCR_STATIC ) && !defined( FREMBED_DLL )
    #if defined( _WIN32 )
        /* By default library for Win32 is DLL. */
        #define FREMBED_DLL
    #else /* !_WIN32 */
        #define MOBILEOCR_STATIC
    #endif /* _WIN32 */
#endif /* !MOBILEOCR_STATIC && !FREMBED_DLL */

#ifndef FINE_API_LINKAGE

    /* FINE_API_IMPORT_SPECIFICATION definition */
    #if defined( MOBILEOCR_STATIC )
        #define FINE_API_IMPORT_SPECIFICATION
    #else
        #ifdef BUILD_MOBILEOCR
            /* Internal compilation */
            #define FINE_API_IMPORT_SPECIFICATION __declspec( dllexport )
        #else
            /* Client compilation */
            #define FINE_API_IMPORT_SPECIFICATION __declspec( dllimport )
        #endif
    #endif

    /* FINE_API_LINKAGE definition */
    #ifdef __cplusplus
        #define FINE_API_LINKAGE extern "C" FINE_API_IMPORT_SPECIFICATION
    #else
        #define FINE_API_LINKAGE FINE_API_IMPORT_SPECIFICATION
    #endif

#endif /* FINE_API_LINKAGE */

#ifndef BIT_FLAG
    #define BIT_FLAG( n ) ( 1 << ( n ) )
#endif

/* ********************************************************************************************* */
/* Data types declarations */

/* Return values for all functions */
typedef enum tagTFineErrorCode {
    /* Completed successfully */
    FEC_NoError = 0,
    /* The library has not been initialized */
    FEC_NotInitialized = 1,
    /* Unacceptable license information is used or the functionality is not available under the license */
    FEC_LicenseError = 2,
    /* One or more arguments are invalid. Use the FineGetLastErrorMessage function for diagnostics */
    FEC_InvalidArgument = 3,
    /* Not enough memory */
    FEC_NotEnoughMemory = 5,
    /* An unspecified internal error */
    FEC_InternalFailure = 6,
    /* The operation was terminated by callback */
    FEC_TerminatedByCallback = 7,
	/* The library has already been initialized */
	FEC_AlreadyInitialized = 8,

    /* Deprecated constants */
    FINE_ERR_NO_ERROR = FEC_NoError,
    FINE_ERR_NOT_INITIALIZED = FEC_NotInitialized,
    FINE_ERR_LICENSE_ERROR = FEC_LicenseError,
    FINE_ERR_INVALID_ARGUMENT = FEC_InvalidArgument,
    FINE_ERR_NOT_ENOUGH_MEMORY = FEC_NotEnoughMemory,
    FINE_ERR_INTERNAL_FAILURE = FEC_InternalFailure,
    FINE_ERR_TERMINATED_BY_CALLBACK = FEC_TerminatedByCallback
} TFineErrorCode;

#pragma deprecated(FINE_ERR_NO_ERROR)
#pragma deprecated(FINE_ERR_NOT_INITIALIZED)
#pragma deprecated(FINE_ERR_LICENSE_ERROR)
#pragma deprecated(FINE_ERR_INVALID_ARGUMENT)
#pragma deprecated(FINE_ERR_NOT_ENOUGH_MEMORY)
#pragma deprecated(FINE_ERR_INTERNAL_FAILURE)
#pragma deprecated(FINE_ERR_TERMINATED_BY_CALLBACK)

/* Deprecated declaration */
typedef TFineErrorCode FINE_ERROR_CODE;

#pragma deprecated(FINE_ERROR_CODE)

/* Image description
 * Presently supported formats:
 *    1 bpp - 0 is black, 1 is white;
 *    8 bpp - 0 is black, 255 is white;
 *    24 bpp - byte order is BGR, (0,0,0) is black, (255,255,255) is white.
 * Lines can start at word or dword boundaries as well, for corresponding ImageWidth,
 * ImageByteWidth and BitsPerPixel. */
typedef struct tagCFineImage {
    /* Width of the image in pixels. */
    int ImageWidth;
    /* Height of the image in pixels. */
    int ImageHeight;
    /* Number of bytes occupied by each raster line. Should be at least
     * ceil(ImageWidth * BitsPerPixel/8) */
    int ImageByteWidth;
    /* Number of bits used for 1 pixel. Should be 1 for black&white, 8 for gray, 24 for color. */
    int BitsPerPixel;
    /* Horizontal and vertical resolution in dpi  */
    int Resolution;
    /* Image bitmap; line-by-line, from top to bottom, each line starts at BYTE boundary */
    BYTE* Image;
} CFineImage;

/* Deprecated declarations */
typedef CFineImage FINE_IMAGE;
typedef CFineImage* PFINE_IMAGE;

#pragma deprecated(FINE_IMAGE)
#pragma deprecated(PFINE_IMAGE)

/* Image processing options */
typedef DWORD TFineImageProcessingOptions;

typedef enum tagTFineImageProcessingOptionsFlags {
    FIPO_Default = 0,

    FIPO_DisableDeskew = BIT_FLAG( 0 ),

    FIPO_DisableImageGeometricTransform = FIPO_DisableDeskew,

    /* Specifies whether the page orientation should be detected.
     * If this flag is set then the rotation parameter of the FineRecognizeImage,
     * FineRecognizeRegion and FineRecognizeBusinessCard functions return the TFineRotationType
     * enumeration value which represents the rotation angle multiple of 90 degrees.
     * FineRecognizeImage, FineRecognizeRegion and FineRecognizeBusinessCard functions return
     * the recognized text coordinates corresponded to a rotated image. If this flag is set
     * then the TFinePrebuiltDataCallbackFunction callback is called before the text recognition.
     * "data" argument of the TFinePrebuiltDataCallbackFunction callback points to the detected
     * value of TFineRotationType type.
     * Note. Setting this flag decreases the recognition speed. */
    FIPO_DetectPageOrientation = BIT_FLAG( 1 ),

    /* Specifies whether the input image has hieroglyphic characters (Chinese, Japanese or Korean).
     * This constant is automatically added into the input parameters of the recognition functions
     * if the input list of the recognition languages of these functions contains an hieroglyphic
     * language. */
    FIPO_HasCjk = BIT_FLAG( 4 ),

    /* If this flag is set, the program will find all text on image.
     * Pictures and garbage will be analyzed and recognized. */
    FIPO_FindAllText = BIT_FLAG( 5 ),

    /* Specifies whether the input image has text that is written in an European and
     * an hieroglyphic languages. This constant is automatically added into input parameters of
     * the FineRecognizeBusinessCard function if the input list of recognition languages of this
     * function contains an hieroglyphic language. */
    FIPO_IsEuropeanWithSomeCjk = BIT_FLAG( 6 ),

    /* If this flag is set, the program will recognize only a horizontal hieroglyphic text
     * on image, a vertical hieroglyphic text will be ignored. */
    FIPO_ProhibitVerticalCjkText = BIT_FLAG( 7 ),

    /* Specifies whether the MICR E13B font must be recognized. */
    FIPO_MicrMode = BIT_FLAG( 8 ),

    /* If this flag is set then after recognition CFineTextLine::Words will point to an array of
     * CFineWordInfo structures with CFineTextLine::WordsCount elements. */
    FIPO_BuildWordsInfo = BIT_FLAG( 9 ),

    /* This flag is used to obtain approximate document layout before the text recognition.
     * If this flag is set then the TFinePrebuiltDataCallbackFunction callback is called
     * before the text recognition. "data" argument of the TFinePrebuiltDataCallbackFunction
     * callback points to a completely filled CFinePrebuiltLayoutInfo structure. */
    FIPO_PrebuildWordsInfo = BIT_FLAG( 10 )
} TFineImageProcessingOptionsFlags;

/* Possible image rotations */
typedef enum tagTFineRotationType {
    FRT_NoRotation,
    FRT_Clockwise,        /* 90 degrees clockwise */
    FRT_UpsideDown,       /* 180 degrees */
    FRT_Counterclockwise, /* 90 degrees counterclockwise */
} TFineRotationType;

/* Angle */
typedef struct tagCFineAngle {
    int Numerator;   /* Numerator of angle tangent */
    int Denominator; /* Denominator of angle tangent */
} CFineAngle;

/* Deprecated declarations */
typedef CFineAngle FINE_ANGLE;
typedef CFineAngle* PFINE_ANGLE;

#pragma deprecated(FINE_ANGLE)
#pragma deprecated(PFINE_ANGLE)

typedef struct tagCFineImageTransformationInfo {
    CFineAngle SkewAngle;
} CFineImageTransformationInfo;

/* Deprecated declarations */
typedef CFineImageTransformationInfo FINE_IMAGE_TRANSFORMATION_INFO;
typedef CFineImageTransformationInfo* PFINE_IMAGE_TRANSFORMATION_INFO;

#pragma deprecated(FINE_IMAGE_TRANSFORMATION_INFO)
#pragma deprecated(PFINE_IMAGE_TRANSFORMATION_INFO)

/* Array of rectangles */
typedef struct tagCFineRects {
    RECT* Rects;    /* Pointer to rectangles buffer */
    int RectsCount; /* Number of rectangles */
} CFineRects;

/* Deprecated declarations */
typedef CFineRects FINE_RECTS;
typedef CFineRects* PFINE_RECTS;

#pragma deprecated(FINE_RECTS)
#pragma deprecated(PFINE_RECTS)

/* Languages' IDs */
typedef enum tagTLanguageID {
    LID_Undefined, /* Special value */
    LID_Afrikaans,
    LID_Albanian,
    LID_Basque,
    LID_Breton,
    LID_Bulgarian,
    LID_Byelorussian,
    LID_Catalan,
    LID_Chechen,
    LID_CrimeanTatar,
    LID_Croatian,
    LID_Czech,
    LID_Danish,
    LID_Dutch,
    LID_DutchBelgian,
    LID_English,
    LID_Estonian,
    LID_Fijian,
    LID_Finnish,
    LID_French,
    LID_German,
    LID_GermanNewSpelling,
    LID_Greek,
    LID_Hawaiian,
    LID_Hungarian,
    LID_Icelandic,
    LID_Indonesian,
    LID_Irish,
    LID_Italian,
    LID_Kabardian,
    LID_Latin,
    LID_Latvian,
    LID_Lithuanian,
    LID_Macedonian,
    LID_Malay,
    LID_Maori,
    LID_Mixed,
    LID_Moldavian,
    LID_Mongol,
    LID_Norwegian,
    LID_NorwegianBokmal,
    LID_NorwegianNynorsk,
    LID_Ossetic,
    LID_Polish,
    LID_Portuguese,
    LID_PortugueseBrazilian,
    LID_Provencal,
    LID_RhaetoRomanic,
    LID_Romanian,
    LID_Russian,
    LID_Samoan,
    LID_Serbian,
    LID_Slovak,
    LID_Slovenian,
    LID_Spanish,
    LID_Swahili,
    LID_Swedish,
    LID_Tagalog,
    LID_Tatar,
    LID_Turkish,
    LID_Ukrainian,
    LID_Welsh,
    LID_Digits,
    LID_WestEuropean,

    LID_FirstCJKLanguageID,
    LID_ChineseSimplified = LID_FirstCJKLanguageID,
    LID_ChineseTraditional,
    LID_Japanese,
    LID_Korean,
    LID_KoreanHangul,
    LID_KoreanHanja,
    LID_LastCJKLanguageID = LID_KoreanHanja,

    /* User languages' IDs count from this */
    LID_FirstUserLanguageID = 1024,

    LID_Max = 0xffff
} TLanguageID;

/* Recognized text representation */

typedef enum tagTFineTextCharacterQuality {
    FTCQ_Min = 0,
    FTCQ_Max = 100
} TFineTextCharacterQuality;

/* Character */
typedef struct tagCFineTextCharacter {
    WCHAR Unicode;          /* Character code as defined by Unicode standard */
    WORD SmallLetterHeight; /* Height of small letter for detected font */
    RECT Rect;              /* Bounding rectangle of character/ligature */
    DWORD Attributes;       /* Combination of FCA_* flags */
    BYTE Quality;           /* Character recognition quality (from FTCQ_Min to FTCQ_Max) */
    BYTE Reserved1;         /* Reserved */
    BYTE Reserved2;         /* Reserved */
    BYTE Reserved3;         /* Reserved */
} CFineTextCharacter;

/* Deprecated declarations */
typedef CFineTextCharacter FINE_TEXT_CHARACTER;
typedef CFineTextCharacter* PFINE_TEXT_CHARACTER;

#pragma deprecated(FINE_TEXT_CHARACTER)
#pragma deprecated(PFINE_TEXT_CHARACTER)

/* Type of the word variant */
typedef enum tagTFineWordVariantType {
    /* Original word as it was recognized. This variant always exist. */
    FWVT_Original,
    /* Primary word form */
    FWVT_PrimaryForm,
} TFineWordVariantType;

/* Word or its derivatives (primary form, corrections) */
typedef struct tagCFineWordVariant {
    /* String representation of the word variant */
    WCHAR* Chars;
    /* The number of Chars */
    int CharCount;
    /* Languages in which the word is */
    TLanguageID* WordLanguages;
    /* The number of WordLanguages */
    int WordLanguagesCount;
    /* The word variant type */
    TFineWordVariantType Type;
} CFineWordVariant;

/* Word attributes */
enum TFineWordAttributes {
    /* Piece of text which is not a word. Can be a gap between words (space, comma), number,
     * a punct set, etc.
     * In this case there should be the only one word variant - with FWVT_Original type. */
    FWA_NotWord = BIT_FLAG( 1 ),
    /* The word which was obtained by cutting the longer word. */
    FWA_SplitWord = BIT_FLAG( 2 ),
    /* The word which was obtained by merging two parts of the word separated by a new line.
     * The word with this flag is always the last one in the text line.
     * You should ignore the rectangle of a word with this attribute. */
    FWA_HyphenatedWord = BIT_FLAG( 3 ),
};

/* Information related to one of the parts of text, obtained after splitting the text by words */
typedef struct tagCFineWordInfo {
    /* The word and its derivatives */
    CFineWordVariant* Variants;
    /* The number of Variants */
    int VariantsCount;
    /* Combination of FWA_* flags */
    DWORD Attributes;
    /* Rectangle of the part of the text */
    RECT Rect;
    /* Medium height of small letters for the word */
    DWORD SmallLetterHeight;
} CFineWordInfo;

/* Text line */
typedef struct tagCFineTextLine {
    CFineTextCharacter* Chars; /* Pointer to characters buffer */
    int CharCount;             /* Number of characters */
    CFineWordInfo* Words;      /* Pointer to word info structures buffer */
    int WordsCount;            /* Number of word info structures */
    RECT Rect;                 /* Bounding rectangle of text line */
    int BaseLine;              /* Coordinate of base line */
} CFineTextLine;

/* Deprecated declarations */
typedef CFineTextLine FINE_TEXT_LINE;
typedef CFineTextLine* PFINE_TEXT_LINE;

#pragma deprecated(FINE_TEXT_LINE)
#pragma deprecated(PFINE_TEXT_LINE)

/* Text block */
typedef struct tagCFineTextBlock {
    /* Pointer to text lines buffer */
    CFineTextLine* Lines;
    /* Number of text lines */
    int LinesCount;
    /* Pointer to region rectangles buffer. Region is union of rectangles set. */
    RECT* RegionRects;
    /* Number of region rectangles */
    int RegionRectsCount;
    /* Attributes of the block. Combination of TFineTextBlockAttributes flags. */
    DWORD Attributes;
} CFineTextBlock;

/* Deprecated declarations */
typedef CFineTextBlock FINE_TEXT_BLOCK;
typedef CFineTextBlock* PFINE_TEXT_BLOCK;

#pragma deprecated(FINE_TEXT_BLOCK)
#pragma deprecated(PFINE_TEXT_BLOCK)

/* Text block attributes. */
typedef enum tagTFineTextBlockAttributes {
    /* Text block is vertical cjk text. */
    FTBA_CjkVertivalBlock = BIT_FLAG( 0 ),
    /* Text block is inverted - white text on black background. */
    FTBA_InvertedTextBlock = BIT_FLAG( 1 )
} TFineTextBlockAttributes;

/* Full recognition result */
typedef struct tagCFineLayout {
    CFineTextBlock* TextBlocks; /* Pointer to text blocks buffer */
    int TextBlocksCount;        /* Number of text blocks */
} CFineLayout;

/* Deprecated declarations */
typedef CFineLayout FINE_LAYOUT;
typedef CFineLayout* PFINE_LAYOUT;

#pragma deprecated(FINE_LAYOUT)
#pragma deprecated(PFINE_LAYOUT)

/* Character attributes */
typedef enum tagTFineCharacterAttributes {
    FCA_Italic = BIT_FLAG( 0 ),
    FCA_Bold = BIT_FLAG( 1 ),
    FCA_Underlined = BIT_FLAG( 2 ),
    FCA_Strikethrough = BIT_FLAG( 3 ),
    FCA_Smallcaps = BIT_FLAG( 4 ),
    FCA_Superscript = BIT_FLAG( 5 ),
    FCA_Uncertain = BIT_FLAG( 16 ),
    /* Attributes for symbols of barcode recognition results:
     * Symbol is binary and is put out in a hex form. */
    FCA_BarcodeBinaryDataHexed = BIT_FLAG( 17 ),
    /* Symbol is binary zero and is replaced with an unknownLetter for a correct display */
    FCA_BarcodeBinaryZero = BIT_FLAG( 18 ),
    /* Symbol is start/stop symbol. Valid for Code39 and Codabar barcode types. */
    FCA_BarcodeStartStopSymbol = BIT_FLAG( 19 ),

    /* Deprecated constants */
    FINE_ATTR_ITALIC = FCA_Italic,
    FINE_ATTR_BOLD = FCA_Bold,
    FINE_ATTR_UNDERLINED = FCA_Underlined,
    FINE_ATTR_STRIKETHROUGH = FCA_Strikethrough,
    FINE_ATTR_SMALLCAPS = FCA_Smallcaps,
    FINE_ATTR_SUPERSCRIPT = FCA_Superscript,
    FINE_ATTR_UNCERTAIN = FCA_Uncertain,
    FINE_ATTR_BARCODE_BINARY_DATA_HEXED = FCA_BarcodeBinaryDataHexed,
    FINE_ATTR_BARCODE_BINARY_ZERO = FCA_BarcodeBinaryZero,
    FINE_ATTR_BARCODE_START_STOP_SYMBOL = FCA_BarcodeStartStopSymbol
} TFineCharacterAttributes;

#pragma deprecated(FINE_ATTR_ITALIC)
#pragma deprecated(FINE_ATTR_BOLD)
#pragma deprecated(FINE_ATTR_UNDERLINED)
#pragma deprecated(FINE_ATTR_STRIKETHROUGH)
#pragma deprecated(FINE_ATTR_SMALLCAPS)
#pragma deprecated(FINE_ATTR_SUPERSCRIPT)
#pragma deprecated(FINE_ATTR_UNCERTAIN)
#pragma deprecated(FINE_ATTR_BARCODE_BINARY_DATA_HEXED)
#pragma deprecated(FINE_ATTR_BARCODE_BINARY_ZERO)
#pragma deprecated(FINE_ATTR_BARCODE_START_STOP_SYMBOL)

/* ROM data pointers */
typedef void* TFinePatternsPtr;
typedef void* TFineDictionaryPtr;
typedef void* TFineKeywordsPtr;

/* Deprecated declarations */
typedef TFinePatternsPtr PFINE_PATTERNS;
typedef TFineDictionaryPtr PFINE_DICTIONARY;
typedef TFineKeywordsPtr PFINE_KEYWORDS;

#pragma deprecated(PFINE_PATTERNS)
#pragma deprecated(PFINE_DICTIONARY)
#pragma deprecated(PFINE_KEYWORDS)

typedef enum tagTFineRecognitionMode {
    FRM_Fast = 0,
    FRM_Full = 1,

    /* Deprecated constants */
    RM_Fast = FRM_Fast,
    RM_Full = FRM_Full
} TFineRecognitionMode;

#pragma deprecated(RM_Fast)
#pragma deprecated(RM_Full)

/* Deprecated declaration */
typedef TFineRecognitionMode TRecognitionMode;

#pragma deprecated(TRecognitionMode)

/* The recognition confidence level */
typedef enum tagTFineRecognitionConfidenceLevel {
    FRCL_Level0 = 0, /* Don't mark characters as uncertain */
    FRCL_Level1 = 1,
    FRCL_Level2 = 2,
    FRCL_Level3 = 3, /* Default level */
    FRCL_Level4 = 4, /* Mark all suspicious characters as uncertain */

    /* Deprecated constants */
    RCL_Level0 = FRCL_Level0,
    RCL_Level1 = FRCL_Level1,
    RCL_Level2 = FRCL_Level2,
    RCL_Level3 = FRCL_Level3,
    RCL_Level4 = FRCL_Level4
} TFineRecognitionConfidenceLevel;

#pragma deprecated(RCL_Level0)
#pragma deprecated(RCL_Level1)
#pragma deprecated(RCL_Level2)
#pragma deprecated(RCL_Level3)
#pragma deprecated(RCL_Level4)

/* Deprecated declaration */
typedef TFineRecognitionConfidenceLevel TRecognitionConfidenceLevel;

#pragma deprecated(TRecognitionConfidenceLevel)

/* Types of business card fields */
typedef enum tagTBcrFieldType {
    BFT_Phone,
    BFT_Fax,
    BFT_Mobile,
    BFT_Email,
    BFT_Web,
    BFT_Address,
    BFT_Name,
    BFT_Company,
    BFT_Job,
    BFT_Text,

    BFT_Count
} TBcrFieldType;

/* Deprecated declaration */
typedef TBcrFieldType TBCRFieldType;

#pragma deprecated(TBCRFieldType)

/* Types of business card field components */
typedef enum tagTBcrComponentType {
    /* Name components: */
    BCT_FirstName,
    BCT_MiddleName,
    BCT_LastName,
    BCT_ExtraName,
    BCT_Title,
    BCT_Degree,
    /* Phone components: */
    BCT_PhonePrefix,
    BCT_PhoneCountryCode,
    BCT_PhoneCode,
    BCT_PhoneBody,
    BCT_PhoneExtension,
    /* Address components: */
    BCT_ZipCode,
    BCT_Country,
    BCT_City,
    BCT_StreetAddress,
    BCT_Region,
    /* Job components: */
    BCT_JobPosition,
    BCT_JobDepartment,

    BCT_Count
} TBcrComponentType;

/* Deprecated declaration */
typedef TBcrComponentType TBCRComponentType;

#pragma deprecated(TBCRComponentType)

/* Component of the business card field */
typedef struct tagCFineBcrFieldComponent {
    TBcrComponentType Type;   /* The component type */
    int TextLinesCount;       /* The number of text lines in the component */
    CFineTextLine* TextLines; /* The text lines of the component */
} CFineBcrFieldComponent;

/* Deprecated declarations */
typedef CFineBcrFieldComponent FINE_FIELD_COMPONENT;
typedef CFineBcrFieldComponent* PFINE_FIELD_COMPONENT;

#pragma deprecated(FINE_FIELD_COMPONENT)
#pragma deprecated(PFINE_FIELD_COMPONENT)

/* Field of business card */
typedef struct tagCFineBcrField {
    /* The field type */
    TBcrFieldType Type;
    /* The number of text lines in the field */
    int TextLinesCount;
    /* The field text lines */
    CFineTextLine* TextLines;
    /* The number of field components. Equals to zero if the field is not divided. */
    int ComponentsCount;
    /* The field components */
    CFineBcrFieldComponent* Components;
} CFineBcrField;

/* Deprecated declarations */
typedef CFineBcrField FINE_BCR_FIELD;
typedef CFineBcrField* PFINE_BCR_FIELD;

#pragma deprecated(FINE_BCR_FIELD)
#pragma deprecated(PFINE_BCR_FIELD)

/* Business card */
typedef struct tagCFineBusinessCard {
    /* A pointer to the field buffer */
    CFineBcrField* Fields;
    /* The number of fields */
    int FieldsCount;
} CFineBusinessCard;

/* Deprecated declarations */
typedef CFineBusinessCard FINE_BUSINESS_CARD;
typedef CFineBusinessCard* PFINE_BUSINESS_CARD;

#pragma deprecated(FINE_BUSINESS_CARD)
#pragma deprecated(PFINE_BUSINESS_CARD)

/* Word sugesstion */
typedef struct tagCFineWordSuggestion {
    /* A pointer to the word buffer. Word is a zero terminated Unicode word. */
    WCHAR** Words;
    /* The number of words */
    int WordsCount;
} CFineWordSuggestion;

/* Deprecated declarations */
typedef CFineWordSuggestion FINE_WORD_SUGGESTION;
typedef CFineWordSuggestion* PFINE_WORD_SUGGESTION;

#pragma deprecated(FINE_WORD_SUGGESTION)
#pragma deprecated(PFINE_WORD_SUGGESTION)

/* Codes of warnings returned through callback function */
typedef enum tagTFineWarningCode {
    FWC_NoWarning,
    FWC_SlowRecognition,
    FWC_ProbablyBadImage,
    FWC_ProbablyWrongLanguages,
    FWC_SureWrongLanguages,

    /* Deprecated constants */
    FINE_WARN_NO_WARNING = FWC_NoWarning,
    FINE_WARN_SLOW_RECOGNITION = FWC_SlowRecognition,
    FINE_WARN_PROBABLY_BAD_IMAGE = FWC_ProbablyBadImage,
    FINE_WARN_PROBABLY_WRONG_LANGUAGES = FWC_ProbablyWrongLanguages,
    FINE_WARN_SURE_WRONG_LANGUAGES = FWC_SureWrongLanguages
} TFineWarningCode;

#pragma deprecated(FINE_WARN_NO_WARNING)
#pragma deprecated(FINE_WARN_SLOW_RECOGNITION)
#pragma deprecated(FINE_WARN_PROBABLY_BAD_IMAGE)
#pragma deprecated(FINE_WARN_PROBABLY_WRONG_LANGUAGES)
#pragma deprecated(FINE_WARN_SURE_WRONG_LANGUAGES)

/* Deprecated declaration */
typedef TFineWarningCode FINE_WARNING_CODE;

#pragma deprecated(FINE_WARNING_CODE)

/* Pointer to callback function for progress information.
 * Return zero to terminate the recognition process.
 * warning is one of TFineWarningCode enum.
 * warningData is a pointer to data structure, which depends on
 *    type of warning. */
typedef int (*TFineProgressCallbackFunction)( int processedPercentage, DWORD warning, void* warningData );

/* Deprecated declaration */
typedef TFineProgressCallbackFunction TCallbackFunction;

#pragma deprecated(TCallbackFunction)

/* Information about text line layout which is available before the textual recognition */
typedef struct tagCFinePrebuiltTextLineInfo {
    /* Text line rectangle */
    RECT Rect;
    /* Pointer to word rectangles buffer. */
    RECT* WordsRects;
    /* The number of word rectangles */
    int WordsRectsCount;
} CFinePrebuiltTextLineInfo;

/* Information about text block layout which is available before the textual recognition */
typedef struct tagCFinePrebuiltTextBlockInfo {
    /* Pointer to region rectangles buffer. Region is union of rectangles set. */
    RECT* RegionRects;
    /* Number of region rectangles */
    int RegionRectsCount;
    /* Pointer to text lines buffer */
    CFinePrebuiltTextLineInfo* Lines;
    /* The number of text lines */
    int LinesCount;
} CFinePrebuiltTextBlockInfo;

/* Information about document layout which is available before the textual recognition */
typedef struct tagCFinePrebuiltLayoutInfo {
/* Pointer to text blocks buffer */
    CFinePrebuiltTextBlockInfo* TextBlocks;
    /* The number of text blocks */
    int TextBlocksCount;
} CFinePrebuiltLayoutInfo;

/* Type of the data which are obtained before the textual recognition.
 * Is used in the TFinePrebuiltDataCallbackFunction callback to specify the nature of the data. */
typedef enum tagTFinePrebuiltDataType {
    /* In TFinePrebuiltDataCallbackFunction callback specifies that the "data" argument should
     * be casted to (TFineRotationType*) pointer type.
     * If FIPO_DetectPageOrientation flag is set then the callback with this data type is used
     * to obtain the detected rotation type. */
    FPDT_RotationType = 0,

    /* In TFinePrebuiltDataCallbackFunction callback specifies that the "data" argument should
     * be casted to (CFinePrebuiltLayoutInfo*) pointer type.
     * If FIPO_PrebuildWordsInfo flag is set then the callback with this data type is used
     * to obtain the preliminray information about the document layout, including the approximate
     * positions of the words.
     * When both FIPO_DetectPageOrientation and FIPO_PrebuildWordsInfo are set, callback with
     * FPDT_WordsInfo data type is called after the callback with FPDT_RotationType data type. */
    FPDT_WordsInfo = 1
} TFinePrebuiltDataType;

/* Pointer to callback function which is used to obtain some data before the textual recognition
 * (see FIPO_PrebuildWordsInfo flag).
 * dataType is a type of the prebuilt data.
 * data is a pointer to a structure, which depends on the type of the prebuilt data.
 *    Must be released with the help of the FineFreeMemory function. */
typedef void (*TFinePrebuiltDataCallbackFunction)( TFinePrebuiltDataType dataType, void* data );

/* Data structure used with FWC_ProbablyWrongLanguages and FWC_SureWrongLanguages warning codes.
 * If MissedLanguages - ID`s of languages, */
typedef struct tagCFineWarningDataWrongLanguages {
    /* List of language IDs, recommended to recognize with. */
    TLanguageID* RecommendedLanguages;
    /* Number of recommended languages. */
    int RecommendedLanguagesCount;
} CFineWarningDataWrongLanguages;

/* Deprecated declaration */
typedef CFineWarningDataWrongLanguages FINE_WARNING_DATA_WRONG_LANGUAGE;
typedef CFineWarningDataWrongLanguages* PFINE_WARNING_DATA_WRONG_LANGUAGE;

#pragma deprecated(FINE_WARNING_DATA_WRONG_LANGUAGE)
#pragma deprecated(PFINE_WARNING_DATA_WRONG_LANGUAGE)

/* A pointer to function for the custom memory managment
 * Allocates memory. Returns 0 if unable to allocate. */
typedef void* (*TFineAllocMemoryFunction)( int size );
/* Releases memory allocated by TFineAllocMemoryFunction */
typedef void (*TFineFreeMemoryFunction)( void* ptr );

/* A pointer to function for executive logging */
typedef void (*TFineExecutionLogFunction)( const WCHAR* str );

/* License information
 * This structure should be filled and passed info FineSetLicenseInfo */
typedef struct tagCFineLicenseInfo {
    /* A pointer to the buffer containing data loaded from license file */
    BYTE* LicenseData;
    /* Length of loaded license file data */
    DWORD DataLength;
    /* String identifying customer application */
    const WCHAR* ApplicationId;
} CFineLicenseInfo;

/* Deprecated declaration */
typedef CFineLicenseInfo FINE_LICENSE_INFO;
typedef CFineLicenseInfo* PFINE_LICENSE_INFO;

#pragma deprecated(FINE_LICENSE_INFO)
#pragma deprecated(PFINE_LICENSE_INFO)

/* Type of barcode */
typedef enum tagTFineBarcodeType {
    FBT_Unrecognized = 0,
    FBT_Code39 = BIT_FLAG( 0 ),
    FBT_Interleaved25 = BIT_FLAG( 1 ),
    FBT_Ean13 = BIT_FLAG( 2 ),
    FBT_Code128 = BIT_FLAG( 3 ),
    FBT_Ean8 = BIT_FLAG( 4 ),
    FBT_Pdf417 = BIT_FLAG( 5 ),
    FBT_Codabar = BIT_FLAG( 6 ),
    FBT_Upce = BIT_FLAG( 7 ),
    FBT_Industrial25 = BIT_FLAG( 8 ),
    FBT_Iata25 = BIT_FLAG( 9 ),
    FBT_Matrix25 = BIT_FLAG( 10 ),
    FBT_Code93 = BIT_FLAG( 11 ),
    FBT_Postnet = BIT_FLAG( 12 ),
    FBT_Ucc128 = BIT_FLAG( 13 ),
    FBT_Patch = BIT_FLAG( 14 ),
    FBT_Aztec = BIT_FLAG( 15 ),
    FBT_Datamatrix = BIT_FLAG( 16 ),
    FBT_Qrcode = BIT_FLAG( 17 ),
    FBT_Upca = BIT_FLAG( 18 ),
    FBT_Maxicode = BIT_FLAG( 19 ),

    /* All one dimensional barcode types */
    FBT_Any1D = FBT_Code39 | FBT_Interleaved25 |
        FBT_Ean13 | FBT_Code128 | FBT_Ean8 | FBT_Codabar | FBT_Upce | FBT_Industrial25 | FBT_Iata25 |
        FBT_Matrix25 | FBT_Code93 | FBT_Ucc128 | FBT_Patch | FBT_Postnet | FBT_Upca,
    /* All two dimensional barcode types */
    FBT_Square2D = FBT_Aztec | FBT_Datamatrix | FBT_Qrcode | FBT_Maxicode,
    /* All one dimensional barcode types, which can have supplement */
    FBT_Any1DWithSupplement = FBT_Ean13 | FBT_Ean8 | FBT_Upce | FBT_Upca,

    /* Deprecated constants */
    FBT_UNRECOGNIZED = FBT_Unrecognized,
    FBT_CODE39 = FBT_Code39,
    FBT_INTERLEAVED25 = FBT_Interleaved25,
    FBT_EAN13 = FBT_Ean13,
    FBT_CODE128 = FBT_Code128,
    FBT_EAN8 = FBT_Ean8,
    FBT_PDF417 = FBT_Pdf417,
    FBT_CODABAR = FBT_Codabar,
    FBT_UPCE = FBT_Upce,
    FBT_INDUSTRIAL25 = FBT_Industrial25,
    FBT_IATA25 = FBT_Iata25,
    FBT_MATRIX25 = FBT_Matrix25,
    FBT_CODE93 = FBT_Code93,
    FBT_POSTNET = FBT_Postnet,
    FBT_UCC128 = FBT_Ucc128,
    FBT_PATCH = FBT_Patch,
    FBT_AZTEC = FBT_Aztec,
    FBT_DATAMATRIX = FBT_Datamatrix,
    FBT_QRCODE = FBT_Qrcode,
    FBT_UPCA = FBT_Upca,
    FBT_MAXICODE = FBT_Maxicode,
    FBT_ANY1D = FBT_Any1D,
    FBT_SQUARE2D = FBT_Square2D,
    FBT_ANY1D_WITH_SUPPLEMENT = FBT_Any1DWithSupplement
} TFineBarcodeType;

#pragma deprecated(FBT_UNRECOGNIZED)
#pragma deprecated(FBT_CODE39)
#pragma deprecated(FBT_INTERLEAVED25)
#pragma deprecated(FBT_EAN13)
#pragma deprecated(FBT_CODE128)
#pragma deprecated(FBT_EAN8)
#pragma deprecated(FBT_PDF417)
#pragma deprecated(FBT_CODABAR)
#pragma deprecated(FBT_UPCE)
#pragma deprecated(FBT_INDUSTRIAL25)
#pragma deprecated(FBT_IATA25)
#pragma deprecated(FBT_MATRIX25)
#pragma deprecated(FBT_CODE93)
#pragma deprecated(FBT_POSTNET)
#pragma deprecated(FBT_UCC128)
#pragma deprecated(FBT_PATCH)
#pragma deprecated(FBT_AZTEC)
#pragma deprecated(FBT_DATAMATRIX)
#pragma deprecated(FBT_QRCODE)
#pragma deprecated(FBT_UPCA)
#pragma deprecated(FBT_MAXICODE)
#pragma deprecated(FBT_ANY1D)
#pragma deprecated(FBT_SQUARE2D)
#pragma deprecated(FBT_ANY1D_WITH_SUPPLEMENT)

/* Barcode orientation */
typedef enum tagTFineBarcodeOrientation {
    FBO_LeftToRight = BIT_FLAG( 0 ),
    FBO_DownToTop = BIT_FLAG( 1 ),
    FBO_RightToLeft = BIT_FLAG( 2 ),
    FBO_TopToDown = BIT_FLAG( 3 ),

    /* All possible orientations */
    FBO_AutoDetect = FBO_LeftToRight | FBO_DownToTop | FBO_RightToLeft | FBO_TopToDown,

    /* Deprecated constants */
    FBO_LEFT_TO_RIGHT = FBO_LeftToRight,
    FBO_DOWN_TO_TOP = FBO_DownToTop,
    FBO_RIGHT_TO_LEFT = FBO_RightToLeft,
    FBO_TOP_TO_DOWN = FBO_TopToDown,
    FBO_AUTODETECT = FBO_AutoDetect
} TFineBarcodeOrientation;

#pragma deprecated(FBO_LEFT_TO_RIGHT)
#pragma deprecated(FBO_DOWN_TO_TOP)
#pragma deprecated(FBO_RIGHT_TO_LEFT)
#pragma deprecated(FBO_TOP_TO_DOWN)
#pragma deprecated(FBO_AUTODETECT)

/* Barcode supplement type.
 * Barcodes types EAN-13, EAN-8, UPC-E, UPC-A can have supplemental barcode
 * to the right from main one.
 * Results of decoding this barcode are added to the main results. */
typedef enum tagTFineBarcodeSupplement {
    FBS_Void = BIT_FLAG( 0 ),
    FBS_2Digit = BIT_FLAG( 1 ),
    FBS_5Digit = BIT_FLAG( 2 ),

    /* All possible options */
    FBS_AutoDetect = FBS_Void | FBS_2Digit | FBS_5Digit,
    /* All non-empty supplement types */
    FBS_AnySupplement = FBS_2Digit | FBS_5Digit,

    /* Deprecated constants */
    FBS_VOID = FBS_Void,
    FBS_2DIGIT = FBS_2Digit,
    FBS_5DIGIT = FBS_5Digit,
    FBS_AUTODETECT = FBS_AutoDetect,
    FBS_ANY_SUPPLEMENT = FBS_AnySupplement
} TFineBarcodeSupplement;

#pragma deprecated(FBS_VOID)
#pragma deprecated(FBS_2DIGIT)
#pragma deprecated(FBS_5DIGIT)
#pragma deprecated(FBS_AUTODETECT)
#pragma deprecated(FBS_ANY_SUPPLEMENT)

/* Codepages currently supported. */
typedef enum tagTFineSupportedCodepage {
    FSC_Arabic = 1256,
    FSC_ArabicIso = 28596,
    FSC_BalticIso = 28594,
    FSC_Cyrillic = 1251,
    FSC_CyrillicIso = 28595,
    FSC_EasternEuropean = 1250,
    FSC_EasternEuropeanIso = 28592,
    FSC_GreekIso = 28597,
    FSC_HebrewIso = 28598,
    FSC_JapanSjis = 932,
    FSC_Latin = 1252,
    FSC_Latin5Iso = 28599,
    FSC_LatinIso = 28591,
    FSC_TurkishIso = 28593,
    FSC_UsMsdos = 437,
    FSC_Utf8 = 65001
} TFineSupportedCodepage;

/* Result of barcode recognition. */
typedef struct tagCFineBarcode {
	/* The barcode type */
    TFineBarcodeType Type;
	/* The barcode text representation */
    CFineTextLine TextLine;
} CFineBarcode;

/* Deprecated declaration */
typedef CFineBarcode FINE_BARCODE;
typedef CFineBarcode* PFINE_BARCODE;

#pragma deprecated(FINE_BARCODE)
#pragma deprecated(PFINE_BARCODE)

/* Image input stream for FineLoadImage function */
typedef struct tagCFineImageInputStream {
	/* Is called to request `size` bytes from the input stream.
	 * Data from the input stream should be written to the `buffer` parameter.
	 * `inputStream` is a pointer to this CFineImageInputStream structure
	 * (like `this` for C++ object).
	 * Returns the number of bytes actually written (may be less than `size`).
	 * Return value that is less or equal to zero is interpreted as an error. */
	int (*Read)( void* inputStream, BYTE* buffer, int size );
	/* Is called to request to skip `size` bytes from the input stream.
	* `inputStream` is a pointer to this CFineImageInputStream structure
	* (like `this` for C++ object).
	 * Returns the number of bytes skipped (should be equal to `size`).
	 * Return value that is not equal to `size` is interpreted as an error.
	 * `Skip` function pointer can be set to zero, then the default implementation,
	 * which uses `Read` function, will be used.*/
	int (*Skip)( void* inputStream, int size );
} CFineImageInputStream;

/* Constants for CFineImageFile::Seek() */
typedef enum tagTFineImageFileSeekPosition {
	FIFSP_Begin,
	FIFSP_Current,
} TFineImageFileSeekPosition;

/* Image file for FineLoadImage function */
typedef struct tagCFineImageFile {
	/* Reads `size` bytes from the file and stores them in the block of memory specified by
	 * `buffer`.
	 * `file` is a pointer to this CFineImageFile structure (like `this` for C++ object).
	 * Returns the number of bytes actually written (may be less than `size`).
	 * Return value that is less or equal to zero is interpreted as an error. */
	int (*Read)( void* file, BYTE* buffer, int size );
	/* Sets the position indicator associated with the file to a new position.
	 * If `from` is equal to FIFSP_Begin, then sets the position indicator to `offset`.
	 * If `from` is equal to FIFSP_Current, then increments the position indicator by `offset`.
	 * `file` is a pointer to this CFineImageFile structure (like `this` for C++ object).
	 * Returns zero, if successful.
	 * Otherwise, returns non-zero value, which is interpreted as an error.
	 */
	int (*Seek)( void* file, int offset, TFineImageFileSeekPosition from );
	/* Returns the file size in bytes. */
	int (*GetLength)( void* file );
} CFineImageFile;

/* Image loading options */
typedef DWORD TFineImageLoadingOptions;

typedef enum tagTFineImageLoadingOptionsFlags {
	/* By default, an image is loaded as is, ignoring any metadata,
	 * The crop rectangle coordinates are in pixels. */
	FILO_Default = 0,
	/* EXIF orientation information is applied to the image. */
	FILO_ApplyExifOrientation = BIT_FLAG( 0 ),
	/* The crop rectangle `cropRect` coordinates are
	 * in hundredths of a percent (in 1/10000) of the original image size. */
	FILO_CropUsingRelativeCoordinates = BIT_FLAG( 1 ),
} TFineImageLoadingOptionsFlags;

/* ********************************************************************************************* */
/* OCR Engine API calls
 * All functions return a combination of TFineErrorCode flags */

/* Initializes library
 * allocFunction [in] is a custom memory allocation function. All memory used by library will be
 *    allocated through this function. If allocFunction is zero, memory will be allocated in
 *    standard way.
 * freeFunction [in] is a custom memory free function. Used to release memory
 *    allocated by allocFunction.
 *    If allocFunction is zero, freeFunction must be zero too.
 * executionLogFunction [in] is a function for executive logging.
 *    If executionLogFunction is zero, logging is disabled.
 */
FINE_API_LINKAGE
TFineErrorCode FineInitialize(
    TFineAllocMemoryFunction allocFunction,
    TFineFreeMemoryFunction freeFunction,
    TFineExecutionLogFunction executionLogFunction
    );

/* Deinitializes library
 */
FINE_API_LINKAGE
TFineErrorCode FineDeinitialize();

/* Sets current license info
 * licenseInfo [in] is a structure containing license information.
 */
FINE_API_LINKAGE
TFineErrorCode FineSetLicenseInfo(
    const CFineLicenseInfo* licenseInfo
    );

/* Allocates memory using allocation functions set by the FineInitialize function
 * size [in] is a size of allocated memory
 * ptr [out] is a pointer to allocated memory
 */
FINE_API_LINKAGE
TFineErrorCode FineAllocMemory( int size, void** ptr );

/* Releases memory allocated for output buffer
 * ptr [in] is a pointer to data allocated by library functions. Can be zero
 */
FINE_API_LINKAGE
TFineErrorCode FineFreeMemory( void* ptr );

/* Gets last error text description
 * message [out] is a pointer to error text. Error text placed in internal library buffer,
 *    so don't release it through FineFreeMemory.
 */
FINE_API_LINKAGE
TFineErrorCode FineGetLastErrorMessage( const WCHAR** message );

/* Gets library version. Full version number is major.minor.modification.build.
 * major [out] is a major version
 * minor [out] is a minor version
 * modification [out] is a modification number
 * build [out] is a build number
  */
FINE_API_LINKAGE
void FineGetVersionInfo(
    int* major,
    int* minor,
    int* modification,
    int* build
    );

/* Gets current license info
 * licenseInfo [out] is a string containing license information.
 *    Must be released through FineFreeMemory.
 */
FINE_API_LINKAGE
TFineErrorCode FineGetLicenseInfo(
    WCHAR** licenseInfo
    );

/* Returns a non-zero value if the library supports 
 * the hieroglyphic language recognition.
 */
FINE_API_LINKAGE
int FineAreCjkLanguagesSupported();

/* Loads an image from the input stream
 * imageDataSource [in] is the input stream of an image file
 * imageLoadingOptions [in] is the bit mask of the image loading options flags
 *    (flags from TFineImageLoadingOptions enumeration).
 * cropRect [in] is a pointer to the image cropping rectangle.
 *    If `cropRect` is zero, image is loaded without crop.
 * imageBuff [out] is the result image object.
 *    Must be released with the help of the FineFreeMemory function.
 */
FINE_API_LINKAGE
TFineErrorCode FineLoadImageFromInputStream(
	CFineImageInputStream* imageInputStream,
	TFineImageLoadingOptions imageLoadingOptions,
	const RECT* cropRect,
	CFineImage** imageBuff
	);

/* Loads an image from a file
 * imageDataSource [in] is the image file
 * imageLoadingOptions [in] is the bit mask of the image loading options flags
 *    (flags from TFineImageLoadingOptions enumeration).
 * cropRect [in] is a pointer to the image cropping rectangle.
 *    If `cropRect` is zero, the image is loaded without crop.
 * imageBuff [out] is the result image object.
 *    Must be released with the help of the FineFreeMemory function.
 */
FINE_API_LINKAGE
TFineErrorCode FineLoadImageFromFile(
	CFineImageFile* imageFile,
	TFineImageLoadingOptions imageLoadingOptions,
	const RECT* cropRect,
	CFineImage** imageBuff
	);

/* Preprocesses image
 * image [in] is an image for preprocessing
 * imageProcessingOptions [in] are image processing options
 * preprocessedImageBuff [out] is the result of preprocessing.
 *    Must be released with the help of the FineFreeMemory function.
 * transformationInfo [out] is an image geometric transformation information
 * progressCallback [in] is a callback function for progress information. Can be zero
 */
FINE_API_LINKAGE
TFineErrorCode FinePreprocessImage(
    const CFineImage* image,
    TFineImageProcessingOptions imageProcessingOptions,
    CFineImage** preprocessedImageBuff,
    CFineImageTransformationInfo** transformationInfo,
    TFineProgressCallbackFunction progressCallback
    );

/* Finds text lines on image
 * image [in] is an image to be analyzed
 * imageProcessingOptions [in] is image processing options
 * linesBuff [out] is line rectangles.
 *    Must be released with the help of the FineFreeMemory function.
 * progressCallback [in] is a callback function for progress information. Can be zero
 */
FINE_API_LINKAGE
TFineErrorCode FineGetTextLines(
    const CFineImage* image,
    TFineImageProcessingOptions imageProcessingOptions,
    CFineRects** linesBuff,
    TFineProgressCallbackFunction progressCallback
    );

/* Get the document layout information, including rectangles of words, without textual recognition.
 * This function doesn't support CJK languages.
 * If any of the CJK languages is set then this function take no effect.
 * languages [in] is a list of language IDs terminated by LID_Undefined
 * patterns [in] is an address of patterns
 * image [in] is an image to be processed
 * imageProcessingOptions [in] is image processing options
 * finePrebuiltLayoutInfo [out] is the result buffer.
 *    Must be released with the help of the FineFreeMemory function.
 * rotation [out] contains the TFineRotationType constant
 *    which specifies the rotation angle of an input image before recognition
 *    if the FIPO_DetectPageOrientation flag is set in the imageProcessingOptions parameter.
 *    Otherwise, it contains FRT_NoRotation.
 *    Can be zero.
 *    Note. The recognized text coordinates are corresponded to a rotated image.
 * progressCallback [in] is a callback function for progress information. Can be zero
 */
FINE_API_LINKAGE
TFineErrorCode FinePrebuildWordsInfo(
    const TLanguageID languages[],
    const TFinePatternsPtr patterns,
    const CFineImage* image,
    TFineImageProcessingOptions imageProcessingOptions,
    CFinePrebuiltLayoutInfo** finePrebuiltLayoutInfo,
    TFineRotationType* rotation,
    TFineProgressCallbackFunction progressCallback
    );

/* Recognizes all text lines on image
 * languages [in] is a list of language IDs terminated by LID_Undefined
 * patterns [in] is an address of patterns
 * cjkPatterns [in] is a zero-terminated list of cjk patterns. Can be zero
 * dictionaries [in] is a zero-terminated list of dictionaries
 * image [in] is an image to be recognized
 * imageProcessingOptions [in] is image processing options
 * recMode [in] is a recognition mode
 * confidenceLevel [in] is a recognition confidence level
 * layoutBuff [out] is the recognition result.
 *    Must be released with the help of the FineFreeMemory function.
 * rotation [out] contains the TFineRotationType constant
 *    which specifies the rotation angle of an input image before recognition
 *    if the FIPO_DetectPageOrientation flag is set in the imageProcessingOptions parameter.
 *    Otherwise, it contains FRT_NoRotation.
 *    Can be zero.
 *    Note. The recognized text coordinates are corresponded to a rotated image.
 * progressCallback [in] is a callback function for progress information. Can be zero
 * prebuiltDataCallback [in] is a callback function which is used to obtain some data before
 *    the textual recognition. Can be zero
 */
FINE_API_LINKAGE
TFineErrorCode FineRecognizeImage(
    const TLanguageID languages[],
    const TFinePatternsPtr patterns,
    const TFinePatternsPtr* cjkPatterns,
    const TFineDictionaryPtr dictionaries[],
    const CFineImage* image,
    TFineImageProcessingOptions imageProcessingOptions,
    TFineRecognitionMode recMode,
    TFineRecognitionConfidenceLevel confidenceLevel,
    CFineLayout** layoutBuff,
    TFineRotationType* rotation,
    TFineProgressCallbackFunction progressCallback,
    TFinePrebuiltDataCallbackFunction prebuiltDataCallback
    );

/* Recognizes text lines in given region (array of rectangles)
 * languages [in] is a list of language IDs terminated by LID_Undefined
 * patterns [in] is an address of patterns
 * cjkPatterns [in] is a zero-terminated list of cjk patterns. Can be zero
 * dictionaries [in] is a zero-terminated list of dictionaries
 * image [in] is an image to be recognized
 * regionLength [in] is a number of rectangles on given region
 * regionRects [in] is an array of rectangles on given region
 * imageProcessingOptions [in] is image processing options
 * recMode [in] is a recognition mode
 * confidenceLevel [in] is a recognition confidence level
 * layoutBuff [out] is tht recognition result.
 *    Must be released with the help of the FineFreeMemory function.
 * rotation [out] contains the TFineRotationType constant
 *    which specifies the rotation angle of an input image before recognition
 *    if the FIPO_DetectPageOrientation flag is set in the imageProcessingOptions parameter.
 *    Otherwise, it contains FRT_NoRotation.
 *    Can be zero.
 *    Note. The recognized text coordinates is corresponded to a rotated image.
 * progressCallback [in] is a callback function for progress information. Can be zero
 * prebuiltDataCallback [in] is a callback function which is used to obtain some data before
 *    the textual recognition. Can be zero
 */
FINE_API_LINKAGE
TFineErrorCode FineRecognizeRegion(
    const TLanguageID languages[],
    const TFinePatternsPtr patterns,
    const TFinePatternsPtr* cjkPatterns,
    const TFineDictionaryPtr dictionaries[],
    const CFineImage* image,
    int regionLength,
    const RECT* regionRects,
    TFineImageProcessingOptions imageProcessingOptions,
    TFineRecognitionMode recMode,
    TFineRecognitionConfidenceLevel confidenceLevel,
    CFineLayout** layoutBuff,
    TFineRotationType* rotation,
    TFineProgressCallbackFunction progressCallback,
    TFinePrebuiltDataCallbackFunction prebuiltDataCallback
    );

/* Finds business card fields in text lines returned from the FineRecognizeImage function
 * keywords [in] is a zero-terminated list of keyword dictionaries
 * layoutBuff [in] is recognized text (the result of the FineRecognizeImage function)
 * businessCardBuffer [out] is business card.
 *    Must be released with the help of the FineFreeMemory function.
 * progressCallback [in] is a callback function for progress information. Can be zero
 */
FINE_API_LINKAGE
TFineErrorCode FineAnalyzeTextAsBusinessCard(
    const TFineKeywordsPtr keywords[],
    const CFineLayout* layoutBuff,
    CFineBusinessCard** businessCardBuffer,
    TFineProgressCallbackFunction progressCallback
    );

/* Recognizes all text lines on image and analyzes image as a business card in one step
 * languages [in] is a list of language IDs terminated by LID_Undefined
 * patterns [in] is an address of patterns
 * cjkPatterns [in] is a zero-terminated list of cjk patterns. Can be zero
 * dictionaries [in] is a zero-terminated list of dictionaries
 * keywords [in] is a zero-terminated list of keyword dictionaries
 * image [in] is an image to be recognized
 * imageProcessingOptions [in] is image processing options
 * recMode [in] is a recognition mode
 * confidenceLevel [in] is a recognition confidence level
 * businessCardBuffer [out] is business card. Must be released through FineFreeMemory.
 * rotation [out] contains the TFineRotationType constant
 *    which specifies the rotation angle of an input image before recognition
 *    if the FIPO_DetectPageOrientation flag is set in the imageProcessingOptions parameter.
 *    Otherwise, it contains FRT_NoRotation.
 *    Can be zero.
 *    Note. The recognized text coordinates is corresponded to a rotated image.
 * progressCallback [in] is a callback function for progress information. Can be zero
 * prebuiltDataCallback [in] is a callback function which is used to obtain some data before
 *    the textual recognition. Can be zero
 */
FINE_API_LINKAGE
TFineErrorCode FineRecognizeBusinessCard(
    const TLanguageID languages[],
    const TFinePatternsPtr patterns,
    const TFinePatternsPtr* cjkPatterns,
    const TFineDictionaryPtr dictionaries[],
    const TFineKeywordsPtr keywords[],
    const CFineImage* image,
    TFineImageProcessingOptions imageProcessingOptions,
    TFineRecognitionMode recMode,
    TFineRecognitionConfidenceLevel confidenceLevel,
    CFineBusinessCard** businessCardBuffer,
    TFineRotationType* rotation,
    TFineProgressCallbackFunction progressCallback,
    TFinePrebuiltDataCallbackFunction prebuiltDataCallback
    );

/* Generates a list of suggestions for the selected word from the specified dictionary.
 * dictionary [in] is an address of dictionary
 * word [in] is a string containing word to get suggestions for
 * wordLength [in] is a number of characters in a given word
 * stringAssurence [in] is an average assurance of symbols in string.
 *    Must be in the range from 0 to 100.
 *    The more stringAssurence, the less variants will be created.
 * suggestionBuff [out] is a word correction suggestion.
 *    Must be released with the help of the FineFreeMemory function.
 */
FINE_API_LINKAGE
TFineErrorCode FineGetWordSuggest (
    const TFineDictionaryPtr dictionary,
    const WCHAR word[],
    int wordLength,
    int stringAssurence,
    CFineWordSuggestion** suggestionBuff
    );

/* Recognizes a given image as a barcode.
 * image [in] is an image to be recognized
 * allowedTypes [in] is a set of barcode types
 * allowedOrientations [in] is a set of barcode orientations
 * allowedSupplements [in] is a set of barcode supplements.
 *    This parameter is ignored for barcode types having no supplements.
 *    If barcode being recognized has no supplement this parameter should be FBS_Void.
 *
 * The following 2 parameters are used for special barcode types,
 *    they are are IGNORED if allowedTypes includes more than one type.
 * hasChecksum [in] should be non zero if barcode being recognized has checksum.
 *    This parameter is valid only for barcodes of the following types:
 *    Code39, Interleaved25, Codabar, Matrix25.
 *    Recognition is considered to be successfull only if checksum is equal
 *    to the value of the last symbol.
 * isCode39WithoutAsterisk [in] should be non zero if Code39 barcode has no start or
 *    stop symbol "*". This parameter is valid only for Code39 barcode.
 *
 * isBinaryInterpretedAsText [in] should be non zero if byte data should be put out as text
 *    in current codepage. Otherwise it will be put out in hex form.
 * defaultCodePage [in] is a codepage for barcodes which mismatch with specification.
 * unknownLetter [in] A character that is written instead of unrecognized symbol or binary zero.
 *    Also it indicates unsuccessful recognition result.
 * resultBuff [out] is a recognition result. Must be released through FineFreeMemory.
 *    If this function fails to recognize given image as a barcode resultBuff will contain only one
 *    unknownLetter symbol with FTCQ_Min quality and have FBT_Unrecognized type.
 * progressCallback [in] is a callback function for progress information. Can be zero
 */
FINE_API_LINKAGE
TFineErrorCode FineRecognizeBarcode(
    const CFineImage* image,
    DWORD allowedTypes,
    DWORD allowedOrientations,
    DWORD allowedSupplements,
    int hasChecksum,
    int isCode39WithoutAsterisk,
    int isBinaryInterpretedAsText,
    TFineSupportedCodepage defaultCodePage,
    WCHAR unknownLetter,
    CFineBarcode** resultBuff,
    TFineProgressCallbackFunction progressCallback
    );

/* ********************************************************************************************* */

#endif /* MobilOCR_h */
