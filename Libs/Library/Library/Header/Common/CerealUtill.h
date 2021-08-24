//cerial
#include "../../cereal/include/cereal/cereal.hpp"
#include "../../cereal/include/cereal/archives/binary.hpp"
#include "../../cereal/include/cereal/archives/portable_binary.hpp"
#include "../../cereal/include/cereal/types/polymorphic.hpp"
#include "../../cereal/include/cereal/types/string.hpp"
#include "../../cereal/include/cereal/types/unordered_map.hpp"
#include "../../cereal/include/cereal/types/map.hpp"
#include "../../cereal/include/cereal/types/vector.hpp"
#include "../../cereal/include/cereal/types/array.hpp"


#define BUTI_REGIST_CBUFFER(T)\
CEREAL_REGISTER_TYPE(ButiEngine::CBuffer_Dx12<T>);\
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::ICBuffer, ButiEngine::CBuffer_Dx12<T>);\


#define BUTI_REGIST_CARRAYBUFFER(T)\
	CEREAL_REGISTER_TYPE(ButiEngine::CArrayBuffer_Dx12<T>);\
	CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::ICBuffer, ButiEngine::CArrayBuffer_Dx12<T>);\



#define BUTI_REGIST_GAMECOMPONENT(T)\
	CEREAL_REGISTER_TYPE(ButiEngine:: T);\
	CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::GameComponent, ButiEngine:: T);\
namespace T{\
	static const ButiEngine::GameComponentRegister  componentregister = ButiEngine::GameComponentRegister(ButiEngine::ObjectFactory::Create <ButiEngine:: T>());\
}

#define BUTI_REGIST_BUTISCRIPTTYPE(T)\
	CEREAL_REGISTER_TYPE(ButiScript::GlobalValueSaveObject< T >);\
	CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiScript::IGlobalValueSaveObject, ButiScript::GlobalValueSaveObject< T >);\

#define BUTI_REGIST_BUTISCRIPTSHAREDTYPE(T)\
	CEREAL_REGISTER_TYPE(ButiScript::GlobalSharedPtrValueSaveObject< T >);\
	CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiScript::IGlobalValueSaveObject, ButiScript::GlobalSharedPtrValueSaveObject< T >);\

