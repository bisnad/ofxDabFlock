/** \file dab_flock_text_tools.h
 */

#ifndef _dab_flock_text_tools_h_
#define _dab_flock_text_tools_h_

#include <array>
#include "dab_singleton.h"
#include "dab_exception.h"
#include "dab_index_map.h"
#include "dab_space_shape.h"
#include "ofVec3f.h"
#include "ofTrueTypeFont.h"


namespace dab
{

namespace flock
{

class TextTools : public Singleton< TextTools >
{
public:
    TextTools();
    ~TextTools();
    
    bool checkFont( const std::string& pFontName );
    bool checkText( const std::string& pTextName );
    space::SpaceShape* text( const std::string& pTextName ) throw (Exception);
    
    
    void createFont( const std::string& pFontName, const std::string& pFontFile, float pSimplifyAmount = 0.01, float pLetterSpacingScale = 1.0, float pWhiteSpacingScale = 1.0, float pLineSpacingScale = 1.0 ) throw (Exception);
    void createText( const std::string& pTextName, const std::string& pText, const std::string& pFontName ) throw (Exception);
    
protected:
    static float sInternalFontSize;
    float mContourSimplifyAmount;
    
    IndexMap< std::string, ofTrueTypeFont* > mFonts;
    IndexMap< std::string, space::SpaceShape* > mTextShapes;
};

};

};

#endif