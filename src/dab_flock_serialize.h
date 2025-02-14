//
//  dab_flock_serialize.h
//  ofxDabFlock
//
//  Created by Daniel Bisig on 01/11/17.
//
//

#ifndef _dab_flock_serialize_h_
#define _dab_flock_serialize_h_

#include <iostream>
#include <map>
#include <array>
#include <Eigen/Dense>
#include "ofVectorMath.h"
#include "ofxJSON.h"
#include "dab_singleton.h"
#include "dab_exception.h"
#include "dab_array.h"

namespace dab
{

namespace flock
{
    
class Behavior;
class SwarmBehavior;

class SerializeTools : public Singleton<SerializeTools>
{
public:
    friend class Singleton<SerializeTools>;
    
    enum SerializeMode
	{
		ConfigMode,
		ValuesMode,
		AllMode
	};

    
    void save( const std::string& pFileName, SerializeMode pMode = AllMode ) throw (Exception);
	void restore( const std::string& pFileName, SerializeMode pMode = AllMode ) throw (Exception);
    
protected:
    std::map< std::string, Behavior* > mBehaviorMap;
    std::map< std::string, SwarmBehavior* > mSwarmBehaviorMap;
    
    SerializeTools();
    ~SerializeTools();
    
    void createBehaviorMap();
    void registerBehavior(Behavior* pBehavior);
    
    void saveConfig( Json::Value& pSerializeData ) throw (Exception);
	void restoreConfig( const Json::Value& pSerializeData ) throw (Exception);
	
	void saveValues( Json::Value& pSerializeData ) throw (Exception);
	void restoreValues( const Json::Value& pSerializeData ) throw (Exception);
    
    void serializeSim( Json::Value& pSerializeData ) throw (Exception);
	void serializeCom( Json::Value& pSerializeData ) throw (Exception);
	void serializeSpace( Json::Value& pSerializeData ) throw (Exception);
	void serializeSwarms( Json::Value& pSerializeData ) throw (Exception);
	void serializeVis( Json::Value& pSerializeData ) throw (Exception);
	void serializeValues( Json::Value& pSerializeData ) throw (Exception);
    
	void deSerializeSim( const Json::Value& pSerializeData ) throw (Exception);
	void deSerializeCom( const Json::Value& pSerializeData ) throw (Exception);
	void deSerializeSpace( const Json::Value& pSerializeData ) throw (Exception);
	void deSerializeSwarms( const Json::Value& pSerializeData ) throw (Exception);
	void deSerializeVis( const Json::Value& pSerializeData ) throw (Exception);
	void deSerializeValues( const Json::Value& pSerializeData ) throw (Exception);
    
    const Json::Value& getValue( const Json::Value& pValue, const std::string& pValueName ) throw (Exception);
    int getInt( const Json::Value& pValue, const std::string& pValueName ) throw (dab::Exception);
    float getFloat( const Json::Value& pValue, const std::string& pValueName ) throw (dab::Exception);
    std::string getString( const Json::Value& pValue, const std::string& pValueName ) throw (dab::Exception);
    
    const Json::Value& getValue( const Json::Value& pValue, int pValueIndex ) throw (dab::Exception);
    int getInt( const Json::Value& pValue, unsigned int pValueIndex ) throw (dab::Exception);
    float getFloat( const Json::Value& pValue, unsigned int pValueIndex ) throw (dab::Exception);
    std::string getString( const Json::Value& pValue, unsigned int pValueIndex ) throw (dab::Exception);
    
    const Json::Value& getValue( const Json::Value& pValue, const std::string& pValueName, int pValueIndex ) throw (dab::Exception);
    int getInt( const Json::Value& pValue, const std::string& pValueName, unsigned int pValueIndex ) throw (dab::Exception);
    float getFloat( const Json::Value& pValue, const std::string& pValueName, unsigned int pValueIndex ) throw (dab::Exception);
    std::string getString( const Json::Value& pValue, const std::string& pValueName, unsigned int pValueIndex ) throw (dab::Exception);

    Json::Value addValues(const Eigen::VectorXf& pValues);
    Json::Value addValues(const glm::vec3& pValues);
    Json::Value addValues(const glm::quat& pValues);
    template<typename ValueType> Json::Value addValues(const Array<ValueType>& pValues);
    template<typename ValueType, int ValueDim> Json::Value addValues(const std::array<ValueType, ValueDim>& pValues);
    
    void getValues(const Json::Value& pData, const std::string& pName, Eigen::VectorXf& pValues ) throw (Exception);
    void getValues(const Json::Value& pData, const std::string& pName, std::array<int, 2>& pValues ) throw (Exception);
    void getValues(const Json::Value& pData, const std::string& pName, std::array<float, 4>& pValues ) throw (Exception);
    void getValues(const Json::Value& pData, const std::string& pName, glm::vec3& pValues ) throw (Exception);
    void getValues(const Json::Value& pData, const std::string& pName, glm::quat& pValues ) throw (Exception);
    template<typename ValueType> void getValues(const Json::Value& pData, const std::string& pName, dab::Array<ValueType>& pValues ) throw (Exception);
};
    
template<typename ValueType>
Json::Value
SerializeTools::addValues(const Array<ValueType>& pValues)
{
    Json::Value valueData;
    
    int valueDim = pValues.size();
    for(int d=0; d<valueDim; ++d)
    {
        valueData.append(pValues[d]);
    }
    
    return valueData;
}
    
template<typename ValueType, int ValueDim>
Json::Value
SerializeTools::addValues(const std::array<ValueType, ValueDim>& pValues)
{
    Json::Value valueData;
    
    int valueDim = pValues.size();
    for(int d=0; d<valueDim; ++d)
    {
        valueData.append(pValues[d]);
    }
    
    return valueData;
}
    
template<typename ValueType>
void
SerializeTools::getValues(const Json::Value& pData, const std::string& pName, dab::Array<ValueType>& pValues ) throw (Exception)
{
    throw Exception("FLOCK ERROR: unsupported data type", __FILE__, __FUNCTION__, __LINE__);
}
    
};
    
};

#endif
