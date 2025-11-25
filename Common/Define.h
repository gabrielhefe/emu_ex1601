#ifndef DEFINE_H
#define DEFINE_H

#pragma warning(disable:4018)

#define ATTR_PRINTF(F,V)

typedef __int64 int64;
typedef int int32;
typedef short int16;
typedef char int8;
typedef unsigned __int64 uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

#if UPDATE_GET_TICK_COUNT == 0
typedef uint32 TCType;
#define MyGetTickCount GetTickCount
#else
typedef uint64 TCType;
#define MyGetTickCount GetTickCount64
#endif

#define SET_NUMBERH(x) ( (uint8)((uint16)(x)>>(uint16)8) )
#define SET_NUMBERL(x) ( (uint8)((uint16)(x) & 0xFF) )
#define MAKE_NUMBERW(x,y)  ( (uint16)(((uint8)((y)&0xFF)) |   ((uint8)((x)&0xFF)<<8 ))  )
#define MAKE_NUMBERDW(x,y) ( (uint32)(((uint16)((y)&0xFFFF)) | ((uint16)((x)&0xFFFF)<<16))  )
#define LODWORD(l) ((uint32)(((uint64)(l)) & 0xffffffff))
#define HIDWORD(l) ((uint32)((((uint64)(l)) >> 32) & 0xffffffff))
#define SET_NUMBERHW(x) ( (uint16)((uint32)(x)>>(uint32)16) )
#define SET_NUMBERLW(x) ( (uint16)((uint32)(x) & 0xFFFF) )

#define CASTLE_SIEGE_SCORE_MACRO(value) ((int64)value & 0xFFFFFFFF)

#if PLATFORM == PLATFORM_WINDOWS
#  define TRINITY_PATH_MAX MAX_PATH
#  define _USE_MATH_DEFINES
#  ifndef DECLSPEC_NORETURN
#    define DECLSPEC_NORETURN __declspec(noreturn)
#  endif //DECLSPEC_NORETURN
#  ifndef DECLSPEC_DEPRECATED
#    define DECLSPEC_DEPRECATED __declspec(deprecated)
#  endif //DECLSPEC_DEPRECATED
#else //PLATFORM != PLATFORM_WINDOWS
#  define TRINITY_PATH_MAX PATH_MAX
#  define DECLSPEC_NORETURN
#  define DECLSPEC_DEPRECATED
#endif //PLATFORM

#if COMPILER == COMPILER_GNU
#  define ATTR_NORETURN __attribute__((noreturn))
#  define ATTR_PRINTF(F, V) __attribute__ ((format (printf, F, V)))
#  define ATTR_DEPRECATED __attribute__((deprecated))
#else //COMPILER != COMPILER_GNU
#  define ATTR_NORETURN
#  define ATTR_PRINTF(F, V)
#  define ATTR_DEPRECATED
#endif //COMPILER == COMPILER_GNU

#define PAIR std::pair

#define CLIENT_TO_SERVER				"[ Client to Server ]"
#define SERVER_TO_CLIENT				"[ Server to Client ]"

#define non_copyable(type) \
	private: \
	type(const type&) = delete; \
	type& operator=(type const&) = delete;

#define DECLARE_PROPERTY_STRING(name)\
	public: \
		std::string m_##name; \
		std::string Get##name() { return this->m_##name; } \
		std::string Get##name() const { return this->m_##name; } \
		char* Get##name##Ptr() { return (char*)this->m_##name.c_str(); } \
		std::string& Get##name##Ref() { return this->m_##name; } \
		void Set##name(std::string const& value) { this->m_##name.assign(value); } \
		void Set##name(const char* value) { this->m_##name.assign(value); } \
		void Reset##name() { this->m_##name.clear(); } \
		void Convert##name##ToLower() { strToLower(this->m_##name); } \
		void Convert##name##ToUpper() { strToUpper(this->m_##name); }


#define DECLARE_BOOL(name)      \
	public: \
		bool m_##name; \
		bool Is##name() const { return this->m_##name; } \
		void Set##name(bool value) { this->m_##name = value; }

#define DECLARE_ARRAY_BOOL(name, size)  \
	public: \
		bool m_##name[size]; \
		bool Is##name(uint32 position) const { return position >= size ? false: this->m_##name[position]; } \
		void Set##name(uint32 position, bool value) { if ( position < size ) this->m_##name[position] = value; } \
		void Set##name(bool value) { for ( uint32 i = 0; i < size; i++ ) this->m_##name[i] = value; }


#define DECLARE_PROPERTY(type, name)    \
	public: \
		##type m_##name; \
		##type Get##name() const { return this->m_##name; } \
		void Set##name(##type value) { this->m_##name = value; } \
		void Increase##name(##type value) { this->m_##name = this->m_##name + value; } \
		void Reduce##name(##type value) { this->m_##name = this->m_##name - value; } \
		void LimitMax##name(##type value) { if ( this->m_##name > value ) this->m_##name = value; } \
		void LimitMin##name(##type value) { if ( this->m_##name < value ) this->m_##name = value; } \
		void And##name(##type value) { this->m_##name = this->m_##name & value; } \
		void Or##name(##type value) { this->m_##name = this->m_##name | value; }

#define DECLARE_FLAG(type, name)        \
	public: \
		##type m_##name; \
		##type Get##name() const { return this->m_##name; } \
		void Set##name(##type value) { this->m_##name = value; } \
		void Add##name(##type value) { this->m_##name = this->m_##name | value; } \
		void Remove##name(##type value) { this->m_##name = this->m_##name & ~value; } \
		bool Is##name(##type value) const { return (this->m_##name & value) != 0; }


#define DECLARE_FLOAT(name)     \
	public: \
		float m_##name; \
		float Get##name() const { return this->m_##name; } \
		void Set##name(float value) { this->m_##name = value; } \
		void Increase##name(float value) { this->m_##name = this->m_##name + value; } \
		void Reduce##name(float value) { this->m_##name = this->m_##name - value; } \
		void LimitMax##name(float value) { if ( this->m_##name > value ) this->m_##name = value; } \
		void LimitMin##name(float value) { if ( this->m_##name < value ) this->m_##name = value; }

#define DECLARE_ENUM(type, name)        \
	public: \
		##type m_##name; \
		##type Get##name() const { return this->m_##name; } \
		void Set##name(##type value) { this->m_##name = value; }

#define DECLARE_RANGE(type, name)       \
	public: \
		##type m_##name##Min; \
		##type m_##name##Max; \
		##type Get##name##Min() const { return this->m_##name##Min; } \
		void Set##name##Min(##type value) { this->m_##name##Min = value; } \
		##type Get##name##Max() const { return this->m_##name##Max; } \
		void Set##name##Max(##type value) { this->m_##name##Max = value; } \
		type GetRandom##name() const { if ( this->Get##name##Min() >= this->Get##name##Max() ) { return this->Get##name##Min(); } \
			return Random<type>((this->Get##name##Max() - this->Get##name##Min()) + 1) + this->Get##name##Min(); }

#define DECLARE_PROPERTY_PTR(type, name)        \
	public: \
		##type* m_##name; \
		##type* Get##name() { return this->m_##name; } \
		##type* Get##name() const { return this->m_##name; } \
		void Set##name(##type* value) { this->m_##name = value; }

#define DECLARE_PROPERTY_ARRAY(type, name, size)        \
	public: \
		##type m_##name[size]; \
		##type* Get##name##s() { return &this->m_##name[0]; } \
		##type const* Get##name##s() const { return &this->m_##name[0]; } \
		##type* Get##name##Ptr(unsigned int dwPosition) { return &this->m_##name[dwPosition]; } \
		##type Get##name(unsigned int dwPosition) { return this->m_##name[dwPosition]; } \
		##type Get##name(unsigned int dwPosition) const { return this->m_##name[dwPosition]; } \
		unsigned int Get##name##Length() const { return size; } \
		void Set##name (unsigned int dwPosition, ##type Value) { this->m_##name[dwPosition] = Value; } \
		void Set##name (##type const* Value) { for ( uint32 i = 0; i < size; ++i ) this->m_##name[i] = Value[i]; } \
		void Reset##name(##type Value) { for ( uint32 i = 0; i < size; ++i ) this->m_##name[i] = Value; } \
		void Increase##name(uint32 dwPosition, ##type value) { this->m_##name[dwPosition] = this->m_##name[dwPosition] + value; } \
		void Reduce##name(uint32 dwPosition, ##type value) { this->m_##name[dwPosition] = this->m_##name[dwPosition] - value; } \
		void Copy##name(##type * value) { for ( uint32 i = 0; i < size; ++i ) value[i] = this->m_##name[i]; }

#define DECLARE_PROPERTY_ARRAY_PTR(type, name, size)    \
	public: \
		##type m_##name[size]; \
		##type* Get##name##s() { return &this->m_##name[0]; } \
		##type* Get##name##Ptr(unsigned int dwPosition) { return &this->m_##name[dwPosition]; } \
		##type Get##name(unsigned int dwPosition) { return this->m_##name[dwPosition]; } \
		##type Get##name(unsigned int dwPosition) const { return this->m_##name[dwPosition]; } \
		unsigned int Get##name##Length() { return size; } \
		void Set##name (unsigned int dwPosition, ##type Value) { this->m_##name[dwPosition] = Value; } \
		void Reset##name(##type Value) { for ( uint32 i = 0; i < size; ++i ) this->m_##name[i] = Value; }

#define DECLARE_ARRAY_STRUCT(type, name, size)  \
	public: \
		##type m_##name[size]; \
		##type* Get##name(unsigned int dwPosition) { return &this->m_##name[dwPosition]; } \
		##type const* Get##name(unsigned int dwPosition) const { return &this->m_##name[dwPosition]; } \
		##type* Get##name##Ptr() { return &this->m_##name[0]; } \
		##type const* Get##name##Ptr() const { return &this->m_##name[0]; } \
		void Set##name(unsigned int dwPosition, type const& value) { if ( dwPosition < size ) { this->m_##name[dwPosition] = value; } }

#define DECLARE_ARRAY_STRUCT_SAFE(type, name, size)     \
	public: \
		##type m_##name[size]; \
		##type* Get##name(unsigned int dwPosition) { return &this->m_##name[dwPosition]; } \
		##type const* Get##name(unsigned int dwPosition) const { return &this->m_##name[dwPosition]; }

#define DECLARE_ARRAY_STRUCT_PTR(type, name, size)      \
	public: \
		##type* m_##name[size]; \
		##type* Get##name(unsigned int dwPosition) const { return this->m_##name[dwPosition]; } \
		void Set##name (unsigned int dwPosition, ##type* Value) { if ( dwPosition < size ) { this->m_##name[dwPosition] = Value; } } \
		void Erase##name() {  for ( uint32 i = 0; i < size; ++i ) SAFE_DELETE(this->m_##name[i]); } \
		void Init##name() {  for ( uint32 i = 0; i < size; ++i ) this->m_##name[i] = NULL; }

#define DECLARE_ARRAY_PTR(type, name)   \
	public: \
		##type** m_##name; \
		uint32 m_##name##size; \
		##type* Get##name(uint32 dwPosition) { return this->m_##name[dwPosition]; } \
		##type const* Get##name(uint32 dwPosition) const { return this->m_##name[dwPosition]; } \
		void Set##name (uint32 dwPosition, ##type* Value) { this->m_##name[dwPosition] = Value; } \
		void Erase##name() { SAFE_DELETE_ARRAY(this->m_##name); this->m_##name##size = 0; } \
		void Create##name(uint32 size) { this->m_##name = new (##type *[size]); for ( uint32 i = 0; i < size; ++i ) this->m_##name[i] = NULL; this->m_##name##size = size; } \
		void Init##name() { this->m_##name = NULL; this->m_##name##size = 0; } \
		uint32 Get##name##Size() const { return this->m_##name##size; }

#define DECLARE_STRUCT_PTR(type, name)  \
	public: \
		##type* m_##name; \
		DECLARE_ENUM(uint32, ##name##Size); \
		##type* Get##name(unsigned int dwPosition) { return &this->m_##name[dwPosition]; } \
		##type const* Get##name(unsigned int dwPosition) const { return &this->m_##name[dwPosition]; } \
		void Set##name (unsigned int dwPosition, ##type Value) { this->m_##name[dwPosition] = Value; } \
		void Set##name (##type * Value) { this->m_##name = Value; } \
		void Erase##name() { SAFE_DELETE_ARRAY(this->m_##name); this->Set##name##Size(0); } \
		void Create##name(uint32 size) { this->m_##name = new (##type[size]); this->Set##name##Size(size); } \
		void Init##name() { this->m_##name = nullptr; this->Set##name##Size(0); } \
		bool Is##name##Loaded() const { return this->m_##name != nullptr; }

#define DECLARE_STRUCT(type, name)      \
	public: \
		##type m_##name; \
		##type* Get##name() { return &this->m_##name; } \
		##type const* Get##name() const { return &this->m_##name; } \
		void Set##name(##type* value) { memcpy(&this->m_##name, value, sizeof(##type)); }

#define DECLARE_PROPERTY_ARRAY_ENUM(type, name, size, Enum)     \
	public: \
		##type m_##name[size]; \
		##type* Get##name##s() { return this->m_##name; } \
		##type* Get##name##Ptr(Enum dwPosition) { return &this->m_##name[dwPosition]; } \
		##type Get##name(Enum dwPosition) const { return this->m_##name[dwPosition]; } \
		void Set##name (Enum dwPosition, ##type Value) { this->m_##name[dwPosition] = Value; } \
		void Reset##name(##type Value) { for ( uint32 i = 0; i < size; ++i ) this->m_##name[i] = Value; } \
		void Increase##name(Enum dwPosition, ##type value) { this->m_##name[dwPosition] = this->m_##name[dwPosition] + value; } \
		void Reduce##name(Enum dwPosition, ##type value) { this->m_##name[dwPosition] = this->m_##name[dwPosition] - value; } \
		void Percent##name(Enum dwPosition, ##type value) { this->Increase##name(dwPosition, this->Get##name(dwPosition) * value / 100); } \
		void Copy##name(##type * value) { for ( uint32 i = 0; i < size; ++i ) value[i] = this->m_##name[i]; } \
		void LimitMax##name(Enum dwPosition, ##type value) { if ( this->m_##name[dwPosition] > value ) this->m_##name[dwPosition] = value; } \
		void LimitMin##name(Enum dwPosition, ##type value) { if ( this->m_##name[dwPosition] < value ) this->m_##name[dwPosition] = value; }

#define DECLARE_PTR(type, name) \
	public: \
		##type* m_##name; \
		##type* Get##name() { return this->m_##name; } \
		##type* Get##name() const { return this->m_##name; } \
		void Set##name(##type* value) { this->m_##name = value; } \
		void Erase##name() { SAFE_DELETE(this->m_##name); }

#define DECLARE_STRING_FIXED(name, size)        \
	public: \
		char m_##name[size]; \
		const char * Get##name() const { return this->m_##name; } \
		void Set##name(const char * value, size_t len) { this->Reset##name(); if ( value ) memcpy(this->m_##name, value, len > size ? size: len); } \
		void Set##name(const char * value) { this->Reset##name(); if ( value ) memcpy(this->m_##name, value, strlen(value) > size ? size: strlen(value)); } \
		void Reset##name() { memset(this->m_##name, 0, size); } \
		void Set##name##ToLower() { StringToLower(this->m_##name); } \
		void Copy##name(const char * value) { this->Reset##name(); if ( value ) strcpy(this->m_##name, value); }

#define DECLARE_ARRAY_ENUM(enum_type, type, name, size) \
	public: \
		##type m_##name[size]; \
		##type Get##name(enum_type dwPosition) const { return this->m_##name[dwPosition]; } \
		void Set##name (enum_type dwPosition, ##type Value) { this->m_##name[dwPosition] = Value; } \
		void Reset##name(##type Value) { for ( uint32 i = 0; i < size; ++i ) this->m_##name[i] = Value; } \
		void Increase##name(enum_type dwPosition, ##type value) { this->m_##name[dwPosition] = this->m_##name[dwPosition] + value; } \
		void Reduce##name(enum_type dwPosition, ##type value) { this->m_##name[dwPosition] = this->m_##name[dwPosition] - value; }
template<typename T>
inline void memcpy(T* target, T const* source)
{
	memcpy(target, source, sizeof(T));
}

#endif
