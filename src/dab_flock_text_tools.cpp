/** \file dab_flock_text_tools.cpp */

#include "dab_flock_text_tools.h"
#include "dab_space_manager.h"
#include "dab_geom_line.h"
#include "dab_geom_geometry_group.h"
#include "dab_space_shape.h"
#include "ofTrueTypeFont.h"

using namespace dab;
using namespace dab::flock;

float TextTools::sInternalFontSize = 1000.0;

TextTools::TextTools()
{}

TextTools::~TextTools()
{
    int fontCount = mFonts.size();
    for(int fI=0; fI<fontCount; ++fI)
    {
        delete mFonts[fI];
    }
	mFonts.clear();
    
    int shapeCount = mTextShapes.size();
    for(int sI=0; sI<shapeCount; ++sI)
    {
        delete mTextShapes[sI];
    }
	mTextShapes.clear();
}

bool
TextTools::checkFont( const std::string& pFontName )
{
	return mFonts.contains( pFontName );
}

bool
TextTools::checkText( const std::string& pTextName )
{
	return mTextShapes.contains( pTextName );
}

space::SpaceShape*
TextTools::text( const std::string& pTextName ) throw (Exception)
{
    if( mTextShapes.contains(pTextName) == false ) throw Exception("FLOCK ERROR: text " + pTextName + " does not exist", __FILE__, __FUNCTION__, __LINE__ );
    
    return mTextShapes[pTextName];
}

void
TextTools::createFont( const std::string& pFontName, const std::string& pFontFile, float pSimplifyAmount, float pLetterSpacingScale, float pWhiteSpacingScale, float pLineSpacingScale ) throw (Exception)
{
    mContourSimplifyAmount = pSimplifyAmount;
    
	if( mFonts.contains( pFontName ) ) throw Exception("FLOCK ERROR: font " + pFontName + " has already been created", __FILE__, __FUNCTION__, __LINE__);
    
    ofTrueTypeFont* _font = new ofTrueTypeFont();

    bool success = _font->load(pFontFile, sInternalFontSize, false, true, true, mContourSimplifyAmount); // buggy, neither vertical flipping nor simplify seem to have any effect on the contours generated
    
    _font->setLetterSpacing(_font->getLetterSpacing() * pLetterSpacingScale);
    _font->setSpaceSize(_font->getLetterSpacing() * pWhiteSpacingScale);
    _font->setLineHeight(_font->getLineHeight() * pLineSpacingScale);
    
    if(success == false)
    {
        delete _font;
        throw Exception("FLOCK ERROR: failed to load file " + pFontFile + " for fond " + pFontName + " has already been created", __FILE__, __FUNCTION__, __LINE__);
    }
    
    mFonts.insert(pFontName, _font);
}

void
TextTools::createText( const std::string& pTextName, const std::string& pText, const std::string& pFontName ) throw (Exception)
{
	if( mTextShapes.contains(pTextName) == true ) throw Exception("FLOCK ERROR: text " + pTextName + " already exists", __FILE__, __FUNCTION__, __LINE__ );
	if( mFonts.contains(pFontName) == false ) throw Exception("FLOCK ERROR: font " + pFontName + " does not exists", __FILE__, __FUNCTION__, __LINE__);
	
	ofTrueTypeFont* font = mFonts[ pFontName ];
    
    std::vector<ofPath> textChars = font->getStringAsPoints(pText, false, false);
    int charCount = textChars.size();

    if(charCount == 0) return;
  
    space::SpaceShape* textShape;
    std::shared_ptr<geom::GeometryGroup> textGroup;
    geom::GeometryGroup* charGroup;
    geom::GeometryGroup* outlineGroup;
    
    textGroup = std::shared_ptr<geom::GeometryGroup>(new geom::GeometryGroup());
    
    for(int cI=0; cI<charCount; ++cI)
    {
		ofPath& textChar = textChars[cI];
		//textChar.setFilled(true);
		//textChar.setPolyWindingMode(OF_POLY_WINDING_ODD);
	
		std::vector<ofPath::Command> pathCommands = textChar.getCommands();
		std::cout << "pathCommands " << pathCommands.size() << "\n";

		textChar.setMode(ofPath::POLYLINES);
        const std::vector<ofPolyline>& charOutlines = textChar.getOutline();

        int outlineCount = charOutlines.size();
        
        std::cout << "cI " << cI << " outlineCount " << outlineCount << "\n";
        
        // check for white space
        if(outlineCount == 0 || (outlineCount == 1 && charOutlines[0].size() == 0 ) ) continue;
        
        charGroup = new geom::GeometryGroup();
        
        for(int oI=0; oI<outlineCount; ++oI)
        {
            //const ofPolyline& charOutline = charOutlines[oI];
            ofPolyline charOutline = charOutlines[oI];

            charOutline.simplify(mContourSimplifyAmount);
            int pointCount = charOutline.size();
            
            std::cout << "cI " << cI << " oI " << oI << " pointCount " << pointCount << "\n";

            outlineGroup = new geom::GeometryGroup();
            
            for(int pI=0; pI<pointCount-1; ++pI)
            {
                ofPoint point1 = charOutline[pI];
                ofPoint point2 = charOutline[pI+1];
                
                if(point1[0] == point2[0] && point1[1] == point2[1] && point1[2] == point2[2]) continue;
                
                point1 *= 1.0 / sInternalFontSize;
                point2 *= 1.0 / sInternalFontSize;
                
                outlineGroup->addGeometry(new geom::Line( point1, point2 ));
                
                //std::cout << "line p1 " << pI << " : " << point1[0] << " " << point1[1] << " " << point1[2] << " -> " << " p2 " << (pI+1) << " :" << point2[0] << " " << point2[1] << " " << point2[2] << "\n";
                
                //std::cout << point1[0] << "\t" << point1[1] << "\n";
                //std::cout <<  point2[0] << "\t" << point2[1] << "\n";
                
             }
            
            ofPoint point1 = charOutline[pointCount-1];
            ofPoint point2 = charOutline[0];
            
            point1 *= 1.0 / sInternalFontSize;
            point2 *= 1.0 / sInternalFontSize;
            
            outlineGroup->addGeometry(new geom::Line( point1, point2 ));
            
            //std::cout << "outlineGroup " << outlineGroup << " lineCount " << outlineGroup->geometryCount() << "\n";
            
            charGroup->addGeometry(outlineGroup);
            
            //std::cout << point1[0] << "\t" << point1[1] << "\n";
            //std::cout <<  point2[0] << "\t" << point2[1] << "\n";
            
            //std::cout << "line p1 " << (pointCount-1) << " : " << point1[0] << " " << point1[1] << " " << point1[2] << " -> " << " p2 " << 0 << " :" << point2[0] << " " << point2[1] << " " << point2[2] << "\n";
            
            //charGroup->addGeometry(outlineGroup);
            
            //textGroup->addGeometry(outlineGroup);
        }
        
        textGroup->addGeometry(charGroup);
    }

	std::cout << "geometryCount " << textGroup->geometryCount() << "\n";
    
    if(textGroup->geometryCount() == 0)
    {
        return;
    }
    
    textGroup->calcMinMaxPos();
    
    std::cout << "textGroup min " << textGroup->minPos() << " max " << textGroup->maxPos() << "\n";
    
    textShape = new space::SpaceShape(textGroup);
    mTextShapes.insert(pTextName, textShape);
}
