#ifndef _TPF_TYPE_UTIL_H
#define _TPF_TYPE_UTIL_H

/*
	Author: Thomas Kim
	First Edit: Jan. 17, 2019
	Second Edit: Feb. 12, 2019
	Third Edit: Mar. 08, 2019
*/

#ifdef _MSVC_LANG  
	#if _MSVC_LANG < 201703L
		#error This libary requires C++17 Standard (Visual Studio 2017).
	#endif
#else

#if __cplusplus < 201703
	#error This library requires C++17 Standard
#endif // end of __cplusplus 

#endif // end of _MSVC_LANG

#include <type_traits>
#include <initializer_list>
#include <functional>
#include <string>
#include <typeinfo>
#include <cstring>
#include <clocale>
#include <utility>
#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <array>
#include <tuple>
#include <iterator>
#include <sstream>
#include <ios>

#include <clocale>
#include <cstring>

// C++17 Standard
#include <any>
#include <variant>
#include <Windows.h>

#ifdef max
#define _TPF_MAX_DEFINED
#pragma push_macro("max")
#undef max
#endif 

#ifdef min
#define _TPF_MIN_DEFINED
#pragma push_macro("min")
#undef min
#endif 

namespace tpf
{
	struct NoTypeDummy{};

	// this function plays very important role
	// in character conversion
	inline void set_default_locale()
	{
		// GetSystemDefaultLocaleName function :
		// https://goo.gl/WLLSG3

		// allocate buffer to hold locale name
		std::wstring locale_name(LOCALE_NAME_MAX_LENGTH, L'\0');

		int locale_legnth =
			GetSystemDefaultLocaleName(
				&locale_name[0],
				LOCALE_NAME_MAX_LENGTH);

		// if failed to get locale name, then just return
		if (locale_legnth == 0) return;

		// https://goo.gl/1A2Hh4
		// set locale of the process or thread
		_wsetlocale(LC_ALL, locale_name.c_str());
	}

	namespace hide
	{
		// convert Windows codepage string to UCS2 (or UTF16) encoding
		// if fails, returns empty string L""
		inline std::wstring cpstr_to_ucs2(const char* cpstr)
		{
			// if cpstr is nullptr, return empty string
			if (cpstr == nullptr) return L"";

			// count of byte of the string to be converted
			int cpstr_len = (int)strlen(cpstr);

			if (cpstr_len < 1) return L"";

			// https://goo.gl/upmy99
			// compute the character length of the string
			// when conversion is successful
			int ucs2_len = MultiByteToWideChar(
				CP_ACP, // CP_ACP works only when the process locale
						// is properly set by calling SetDefaultLocale()
				0, cpstr, cpstr_len, NULL, 0);

			// we failed to compute the character length
			// of the string when conversion is successful
			if (ucs2_len == 0) return L"";

			// prepare buffer to hold converted string
			// do not +1 to ucs2_len, because std::wstring
			// manages terminating null internally
			std::wstring ucs2_str(ucs2_len, L'\0');

			int converted = MultiByteToWideChar(
				CP_ACP, // CP_ACP works only when the process locale
						// is properly set by calling SetDefaultLocale()
				0, cpstr, cpstr_len,
				&ucs2_str[0], ucs2_len);

			// failed to convert
			if (converted == 0)
				return L"";
			else
				return ucs2_str;
		}

		// convert from UCS2 (or UTF16) to Windows copdepage encoding
		// if fails, returns empty string ""
		inline std::string ucs2str_to_cpstr(const wchar_t* ucs2str)
		{
			if (ucs2str == nullptr) return "";

			// count of character of UCS2 string
			int ucs2_len = (int)wcslen(ucs2str);

			if (ucs2_len < 1) return "";

			// https://goo.gl/acoQBt
			int cpstr_len = WideCharToMultiByte(
				CP_ACP, // this CP_ACP means
						// we want to convert process (or thread)
						// codepage text to UCS2
				0, // dwFlag
				ucs2str,
				ucs2_len, NULL, 0, NULL, NULL);

			// we failed to compute the byte count of the string
			// when the conversion is successful
			if (cpstr_len == 0) return "";

			// prepare a string buffer to
			// hold the converted string
			// do not +1 to cpstr_len,
			// because std::string manages terminating null
			std::string cpstr(cpstr_len, '\0');

			int converted = WideCharToMultiByte(
				CP_ACP, 0, ucs2str, ucs2_len, &cpstr[0],
				cpstr_len, NULL, NULL);

			if (converted == 0)
				return "";
			else
				return  cpstr;
		}

		// convert from UCS2 (or UTF16) to UTF8 encoding
		// if fails, returns empty string ""
		inline std::string ucs2str_to_utf8str(const wchar_t* ucs2str)
		{
			if (ucs2str == nullptr) return "";

			// count of character of UCS2 string
			int ucs2_len = (int)wcslen(ucs2str);

			if (ucs2_len < 1) return "";

			// https://goo.gl/acoQBt
			int cpstr_len = WideCharToMultiByte(
				CP_UTF8,
				0, // dwFlag
				ucs2str,
				ucs2_len, NULL, 0, NULL, NULL);

			// we failed to compute the byte count of the string
			// when the conversion is successful
			if (cpstr_len == 0) return "";

			// prepare a string buffer to
			// hold the converted string
			// do not +1 to cpstr_len,
			// because std::string manages terminating null
			std::string cpstr(cpstr_len, '\0');

			int converted = WideCharToMultiByte(
				CP_UTF8, 0, ucs2str, ucs2_len, &cpstr[0],
				cpstr_len, NULL, NULL);

			if (converted == 0)
				return "";
			else
				return  cpstr;
		}

		// convert UTF8 string to UCS2 (or UTF16) encoding
		// if fails, returns empty string L""
		inline std::wstring utf8str_to_ucs2(const char* cpstr)
		{
			// if cpstr is nullptr, return empty string
			if (cpstr == nullptr) return L"";

			// count of byte of the string to be converted
			int cpstr_len = (int)strlen(cpstr);

			if (cpstr_len < 1) return L"";

			// https://goo.gl/upmy99
			// compute the character length of the string
			// when conversion is successful
			int ucs2_len = MultiByteToWideChar(
				CP_UTF8, 0, cpstr, cpstr_len, NULL, 0);

			// we failed to compute the character length
			// of the string when conversion is successful
			if (ucs2_len == 0) return L"";

			// prepare buffer to hold converted string
			// do not +1 to ucs2_len, because std::wstring
			// manages terminating null internally
			std::wstring ucs2_str(ucs2_len, L'\0');

			int converted = MultiByteToWideChar(
				CP_UTF8, 0, cpstr, cpstr_len,
				&ucs2_str[0], ucs2_len);

			// failed to convert
			if (converted == 0)
				return L"";
			else
				return ucs2_str;
		}

		// convert Windows codepage to UTF8
		inline std::string cpstr_to_utf8str(const char* cpstr)
		{
			/*
			We cannot convert directly from
			codepage to UTF8 .... instead

			We convert
			codepage to UCS2
			then UCS2 to UTF8
			*/

			// if nullptr, return empty string ""
			if (cpstr == nullptr) return "";

			std::wstring ucs2str = cpstr_to_ucs2(cpstr);

			// we failed to convert from codepage encoding to UCS2
			if (ucs2str == L"") return "";

			// converting from UCS2 to UTF8
			return ucs2str_to_utf8str(ucs2str.c_str());
		}

		// convert UTF8 to codepage encoding
		inline std::string utf8str_to_cpstr(const char* utf8str)
		{
			/*
			We cannot convert directly from
			UTF8 to codepage encoding... instead

			We convert
			UTF8 to UCS2
			then UCS2 to codepage encoding
			*/

			if (utf8str == nullptr) return "";

			std::wstring ucs2str = utf8str_to_ucs2(utf8str);

			// if conversion fails, return empty string ""
			if (ucs2str == L"") return "";

			return ucs2str_to_cpstr(ucs2str.c_str());
		}
	
		template<typename CharTarget, typename CharSource>
		struct string_conversion_st;

		// wchar_t to char
		// wide char to code page
		template<> struct
			string_conversion_st<char, wchar_t>
		{
			using target_char_t = char;
			using target_str_t = std::basic_string<target_char_t>;

			using source_char_t = wchar_t;
			using source_str_t = std::basic_string<source_char_t>;

			static target_str_t convert(const source_char_t* src_str)
			{
				return ucs2str_to_cpstr(src_str);
			}

			static target_str_t convert(const source_char_t src)
			{
				source_str_t src_str((size_t)1, src);
				return ucs2str_to_cpstr(src_str.c_str());
			}

			static target_str_t convert(const source_str_t& src_str)
			{
				return ucs2str_to_cpstr(src_str.c_str());
			}
		};

		struct StringConversion
		{
			static bool convert(std::wstring& wst_tgt, const char* char_src)
			{
				wst_tgt = tpf::hide::cpstr_to_ucs2(char_src);
				return !wst_tgt.empty();
			}

			static bool convert(std::string& str_tgt, const char* char_src)
			{
				str_tgt = std::string(char_src); return true;
			}

			static bool convert(std::string& str_tgt, const wchar_t* wch_src)
			{
				str_tgt = hide::ucs2str_to_cpstr(wch_src);
				return !str_tgt.empty();
			}

			static bool convert(std::wstring& wst_tgt, const wchar_t* wch_src)
			{
				wst_tgt = std::wstring(wch_src); return true;
			}

			static bool convert(...) { return false; }
		};

		// char to wchar_t
		// code page to wide char
		template<> struct
			string_conversion_st<wchar_t, char>
		{
			using target_char_t = wchar_t;
			using target_str_t = std::basic_string<target_char_t>;

			using source_char_t = char;
			using source_str_t = std::basic_string<source_char_t>;

			static target_str_t convert(const source_char_t* src_str)
			{
				return cpstr_to_ucs2(src_str);
			}

			static target_str_t convert(const source_char_t src)
			{
				source_str_t src_str(size_t(1), src);

				return cpstr_to_ucs2(src_str.c_str());
			}

			static target_str_t convert(const source_str_t& src_str)
			{
				return cpstr_to_ucs2(src_str.c_str());
			}
		};	
	}
	
	template<typename T>
	std::string type_to_string()
	{
#if defined(_MSC_VER)

		std::string type_name(__FUNCSIG__);
		auto start_pos = type_name.find_first_of('<',
			std::string(typeid(std::string).name()).size()) + 1;
		auto end_pos = type_name.find_last_of('>');

		return type_name.substr(start_pos, (end_pos - start_pos));

#elif defined(__clang__)
		std::string type_name(__PRETTY_FUNCTION__);
		auto start_pos = type_name.find_first_of('=') + 2;
		auto end_pos = type_name.find_first_of(']', start_pos);
		return type_name.substr(start_pos, (end_pos - start_pos));

#elif defined(__GNUC__)
		std::string type_name(__PRETTY_FUNCTION__);
		auto start_pos = type_name.find_first_of('=') + 2;
		auto end_pos = type_name.find_first_of(';', start_pos);
		return type_name.substr(start_pos, (end_pos - start_pos));
#else
		return "Failed To Convert to String";
#endif

	} // std::string type_to_string()
	
	template<typename T, size_t ArrayCount>
	auto decay_value(const T(&v)[ArrayCount]) { return v; }

	template<typename T> auto decay_v(T v) { return v; }
	   
	namespace hide
	{
		//  primary template class
		template<class _Ty>
		struct st_remove_const
		{
			using type = _Ty;
		};

		// specialization const _Ty
		template<class _Ty>
		struct st_remove_const<const _Ty>
		{
			using type = _Ty;
		};

		// specialization const _Ty
		template<class _Ty>
		struct st_remove_const<const _Ty&>
		{
			using type = _Ty & ;
		};

		// specialization const _Ty
		template<class _Ty>
		struct st_remove_const<const _Ty&&>
		{
			using type = _Ty && ;
		};

		template<class _Ty>
		using remove_const_helper = typename st_remove_const<_Ty>::type;

		template<typename RefType>
		using add_const_ref_helper = std::conditional_t<
			std::is_lvalue_reference_v<RefType>,
			std::add_lvalue_reference_t<std::add_const_t<std::remove_reference_t<RefType>>>,
			std::add_rvalue_reference_t<std::add_const_t<std::remove_reference_t<RefType>>>>;

		template<typename T>
		using add_const_helper = std::conditional_t<
			std::is_reference_v<T>,
			add_const_ref_helper<T>, std::add_const_t<T>>;

		template<typename T>
		constexpr bool is_const_helper = std::is_const_v<std::remove_reference_t<T>>;

		template<typename T>
		constexpr bool is_lvalue_helper = std::is_lvalue_reference<T>::value;

		template<typename T>
		constexpr bool is_xvalue_helper = std::is_rvalue_reference<T>::value;

		template<typename T>
		constexpr bool is_prvalue_helper = !(is_lvalue_helper<T> || is_xvalue_helper<T>);

		template<typename T>
		constexpr bool is_rvalue_helper = is_xvalue_helper<T> || is_prvalue_helper<T>;

		template<typename T>
		constexpr bool is_glvalue_helper = is_xvalue_helper<T> || is_lvalue_helper<T>;
	
	} // end of namespace hide

} // end of namespace tpf

#define Tpf_GetTypeName(type_name)  tpf::type_to_string<type_name>()
#define Tpf_GetTypeCategory(type_instance) tpf::type_to_string<decltype(type_instance)>()
#define Tpf_GetValueCategory(type_instance) tpf::type_to_string<decltype((type_instance))>()

#define Tpf_IsTypeConst(type_name) tpf::hide::is_const_helper<type_name>
#define Tpf_AddTypeConst(type_name) tpf::hide::add_const_helper<type_name>
#define Tpf_RemoveTypeConst(type_name) tpf::hide::remove_const_helper<type_name>

#define Tpf_IsLvalue(type_instance) tpf::hide::is_lvalue_helper<decltype((type_instance))>
#define Tpf_IsXvalue(type_instance) tpf::hide::is_xvalue_helper<decltype((type_instance))>
#define Tpf_IsPRvalue(type_instance) tpf::hide::is_prvalue_helper<decltype((type_instance))>
#define Tpf_IsRvalue(type_instance) tpf::hide::is_rvalue_helper<decltype((type_instance))>
#define Tpf_IsGLvalue(type_instance) tpf::hide::is_glvalue_helper<decltype((type_instance))>

#define Tpf_IsConst(type_instance) tpf::hide::is_const_helper<decltype(type_instance)>

namespace tpf
{
	namespace hide
	{
		template<typename... List> struct type_list_st {};
	}

	template<typename... List>
	using type_list_t = hide::type_list_st<List...>;
	
	template<typename... List>
	constexpr auto type_list_v = type_list_t<List...>{};

	template<typename T> using base_t =
		std::remove_const_t<std::remove_reference_t
		<std::remove_pointer_t<std::decay_t<std::remove_extent_t<T>>>>>;

	template<typename T> using flat_base_t =
		std::remove_const_t<std::remove_reference_t
		<std::remove_pointer_t<std::decay_t<std::remove_all_extents_t<T>>>>>;

	template<typename... Types>
	constexpr decltype(auto) type_get_list(Types&&... args)
	{
		return type_list_t<decltype(std::forward<Types>(args))...>{};
	}

	template<typename... Types>
	constexpr decltype(auto) type_get_decayed_list(Types... args)
	{
		return type_list_t<decltype(args)...>{};
	}

	////////////////////////////////////////
	struct boolean
	{
		static constexpr bool Left = false;
		static constexpr bool Right = true;

		static constexpr bool Or = false;
		static constexpr bool And = true;

		static constexpr bool No = false;
		static constexpr bool Yes = true;

		static constexpr bool False = false;
		static constexpr bool True = true;

		static constexpr bool Before = false;
		static constexpr bool After = true;

		static constexpr bool Prepend = false;
		static constexpr bool Append = true;
	};

	namespace hide
	{
		template<template<typename, typename> class BinaryOpr, typename ArgType, typename Type, typename... Types>
		struct find_type_st
		{
			static constexpr bool value = find_type_st<BinaryOpr, ArgType, Type>::value ? true :
				find_type_st<BinaryOpr, ArgType, Types...>::value;

			using type = std::conditional_t< find_type_st<BinaryOpr, ArgType, Type>::value,
				Type, typename find_type_st<BinaryOpr, ArgType, Types...>::type>;
		};

		template<template<typename, typename> class BinaryOpr, typename ArgType, typename Type>
		struct find_type_st<BinaryOpr, ArgType, Type>
		{
			static constexpr bool value = BinaryOpr<Type, ArgType>::value;

			using type = std::conditional_t<value, Type, NoTypeDummy>;
		};

		template<template<typename, typename> class BinaryOpr, typename ArgType, typename Type, typename... Types>
		struct find_type_st<BinaryOpr, ArgType, tpf::type_list_t<Type, Types...>>
		{
			static constexpr bool value = find_type_st<BinaryOpr, ArgType, Type, Types...>::value;

			using type = typename find_type_st<BinaryOpr, ArgType, Type, Types...>::type;
		};

		template<template<typename, typename> class BinaryOpr, typename ArgType, typename Type>
		struct find_type_st<BinaryOpr, ArgType, tpf::type_list_t<Type>>
		{
			static constexpr bool value = find_type_st<BinaryOpr, ArgType, Type>::value;

			using type = typename find_type_st<BinaryOpr, ArgType, Type>::type;
		};

	} // end of namespace hide

	template<typename ArgType, typename Type, typename... Types>
	constexpr auto same_type_v = hide::find_type_st<std::is_same, ArgType, Type, Types...>::value;

	template<typename ArgType, typename Type, typename... Types>
	using same_type_t = typename hide::find_type_st<std::is_same, ArgType, Type, Types...>::type;

	template<typename ArgType, typename Type, typename... Types>
	constexpr auto constructible_type_v = hide::find_type_st<std::is_constructible, ArgType, Type, Types...>::value;

	template<typename ArgType, typename Type, typename... Types>
	using constructible_type_t = typename hide::find_type_st<std::is_constructible, ArgType, Type, Types...>::type;

	namespace hide
	{
		template<typename ArgType, typename Type, typename... Types>
		struct find_best_type_st
		{
			static constexpr bool value = same_type_v<ArgType, Type, Types...> ? true :
				constructible_type_v<ArgType, Type, Types...>;

			using type = std::conditional_t< same_type_v<ArgType, Type, Types...>,
				same_type_t<ArgType, Type, Types...>,
				constructible_type_t<ArgType, Type, Types...>>;
		};

	} // end of namespace hide

	template<typename ArgType, typename Type, typename... Types>
	constexpr auto best_type_v = hide::find_best_type_st<ArgType, Type, Types...>::value;

	template<typename ArgType, typename Type, typename... Types>
	using best_type_t = typename hide::find_best_type_st<ArgType, Type, Types...>::type;

	namespace hide
	{
		template<bool OrAnd,
			bool LeftRight,
			typename Type,
			template<typename, typename> class BinaryPredicate,
			typename ListHead, typename... ListTails>
			struct type_over_set_st;

		template<bool LeftRight,
			typename Type,
			template<typename, typename> class BinaryPredicate,
			typename ListHead, typename... ListTails>
			struct type_over_set_st<boolean::Or, LeftRight, Type,
			BinaryPredicate, ListHead, ListTails...>
		{
			static constexpr bool value =
				type_over_set_st<boolean::Or, LeftRight, Type, BinaryPredicate, ListHead>::value
				|| type_over_set_st<boolean::Or, LeftRight, Type, BinaryPredicate, ListTails...>::value;
		};

		template<bool LeftRight,
			typename Type,
			template<typename, typename> class BinaryPredicate,
			typename ListHead, typename... ListTails>
			struct type_over_set_st<boolean::And, LeftRight, Type,
			BinaryPredicate, ListHead, ListTails...>
		{
			static constexpr bool value =
				type_over_set_st<boolean::And, LeftRight, Type, BinaryPredicate, ListHead>::value
				&& type_over_set_st<boolean::And, LeftRight, Type, BinaryPredicate, ListTails...>::value;
		};

		///////////////////////////////////////////////////
		template<typename Type,
			template<typename, typename> class BinaryPredicate,
			typename ListHead, typename... ListTails>
			struct type_over_set_st<boolean::Or, boolean::Left, Type,
			BinaryPredicate, ListHead, ListTails...>
		{
			static constexpr bool value =
				type_over_set_st<boolean::Or, boolean::Left, Type, BinaryPredicate, ListHead>::value
				|| type_over_set_st<boolean::Or, boolean::Left, Type, BinaryPredicate, ListTails...>::value;
		};

		template<typename Type,
			template<typename, typename> class BinaryPredicate,
			typename ListHead, typename... ListTails>
			struct type_over_set_st<boolean::And, boolean::Left, Type,
			BinaryPredicate, ListHead, ListTails...>
		{
			static constexpr bool value =
				type_over_set_st<boolean::And, boolean::Left, Type, BinaryPredicate, ListHead>::value
				&& type_over_set_st<boolean::And, boolean::Left, Type, BinaryPredicate, ListTails...>::value;
		};

		template<typename Type,
			template<typename, typename> class BinaryPredicate,
			typename ListHead, typename... ListTails>
			struct type_over_set_st<boolean::Or, boolean::Right, Type,
			BinaryPredicate, ListHead, ListTails...>
		{
			static constexpr bool value =
				type_over_set_st<boolean::Or, boolean::Right, Type, BinaryPredicate, ListHead>::value
				|| type_over_set_st<boolean::Or, boolean::Right, Type, BinaryPredicate, ListTails...>::value;
		};

		template<typename Type,
			template<typename, typename> class BinaryPredicate,
			typename ListHead, typename... ListTails>
			struct type_over_set_st<boolean::And, boolean::Right, Type,
			BinaryPredicate, ListHead, ListTails...>
		{
			static constexpr bool value =
				type_over_set_st<boolean::And, boolean::Right, Type, BinaryPredicate, ListHead>::value
				&& type_over_set_st<boolean::And, boolean::Right, Type, BinaryPredicate, ListTails...>::value;
		};

		//////////////////////////////////////////

		template<typename Type,
			template<typename, typename> class BinaryPredicate, typename ListHead>
		struct type_over_set_st<boolean::Or, boolean::Left, Type, BinaryPredicate, ListHead>
		{
			static constexpr bool value = BinaryPredicate<Type, ListHead>::value;
		};

		template<typename Type,
			template<typename, typename> class BinaryPredicate,
			typename ListHead>
			struct type_over_set_st<boolean::And, boolean::Left, Type, BinaryPredicate, ListHead>
		{
			static constexpr bool value = BinaryPredicate<Type, ListHead>::value;
		};

		template<typename Type,
			template<typename, typename> class BinaryPredicate, typename ListHead>
		struct type_over_set_st<boolean::Or, boolean::Right, Type, BinaryPredicate, ListHead>
		{
			static constexpr bool value = BinaryPredicate<ListHead, Type>::value;
		};

		template<typename Type,
			template<typename, typename> class BinaryPredicate, typename ListHead>
		struct type_over_set_st<boolean::And, boolean::Right, Type, BinaryPredicate, ListHead>
		{
			static constexpr bool value = BinaryPredicate<ListHead, Type>::value;
		};

		template<typename Type, typename... Types>
		struct concate_type_st
		{
			using type = tpf::type_list_t<Type, Types...>;
		};

		template<typename Head, typename...Tails, typename... Types>
		struct concate_type_st<tpf::type_list_t<Head, Tails...>, Types...>
		{
			using type = tpf::type_list_t<Head, Tails..., Types...>;
		};

		template<typename Type>
		struct concate_type_st<tpf::type_list_t<Type>>
		{
			using type = tpf::type_list_t<Type>;
		};

		template<typename Head, typename... Tails>
		struct concate_type_st<tpf::type_list_t<Head, Tails...>>
		{
			using type = tpf::type_list_t<Head, Tails...>;
		};

		template<typename Type1, typename Type2>
		struct concate_type_st<Type1, Type2>
		{
			using type = tpf::type_list_t<Type1, Type2>;
		};

		template<typename Type, typename Head2, typename... Tails2>
		struct concate_type_st<Type, tpf::type_list_t<Head2, Tails2...>>
		{
			using type = tpf::type_list_t<Type, Head2, Tails2...>;
		};

		template<typename Head1, typename... Tails1, typename Type2, typename... Tails2>
		struct concate_type_st<tpf::type_list_t<Head1, Tails1...>, Type2, Tails2...>
		{
			using type = tpf::type_list_t<Head1, Tails1..., Type2, Tails2...>;
		};

		template<typename Head1, typename... Tails1,
			typename Head2, typename... Tails2>
			struct concate_type_st<tpf::type_list_t<Head1, Tails1...>, tpf::type_list_t<Head2, Tails2...>>
		{
			using type = tpf::type_list_t<Head1, Tails1..., Head2, Tails2...>;
		};
	}

	template<typename Type, typename... Types>
	using concate_type_t = typename hide::concate_type_st<Type, Types...>::type;

	////////////////////////////////////////

	namespace hide
	{
		template<bool OrAnd,
			bool LeftRight,
			typename Type,
			template<typename, typename, typename...> class BinaryPredicate,
			typename ListHead, typename... ListTails>
			struct type_opr_over_set_st
		{
			static constexpr bool value =
				type_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate, ListHead, ListTails...>::value;
		};

		template<bool OrAnd,
			bool LeftRight,
			typename Type,
			template<typename, typename, typename...> class BinaryPredicate,
			typename ListHead, typename... ListTails>
			struct type_opr_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate,
			tpf::type_list_t<ListHead, ListTails...>>
		{
			static constexpr bool value =
				type_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate, ListHead, ListTails...>::value;
		};

		template<bool OrAnd,
			bool LeftRight,
			typename Type,
			template<typename, typename, typename...> class BinaryPredicate,
			typename Head, typename ListHead, typename... ListTails>
			struct type_opr_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate,
			Head, tpf::type_list_t<ListHead, ListTails...>>
		{
			static constexpr bool value =
				type_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate,
				Head, ListHead, ListTails...>::value;
		};

		template<bool OrAnd,
			bool LeftRight,
			typename Type,
			template<typename, typename, typename...> class BinaryPredicate,
			typename ListHead, typename... ListTails, typename Head, typename... Tails>
			struct type_opr_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate,
			tpf::type_list_t<ListHead, ListTails...>, Head, Tails...>
		{
			static constexpr bool value =
				type_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate,
				ListHead, ListTails..., Head, Tails...>::value;
		};

		template<bool OrAnd,
			bool LeftRight,
			typename Type,
			template<typename, typename, typename...> class BinaryPredicate,
			typename ListHead, typename... ListTails, typename Head, typename... Tails>
			struct type_opr_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate,
			tpf::type_list_t<ListHead, ListTails...>, tpf::type_list_t<Head, Tails...>>
		{
			static constexpr bool value =
				type_over_set_st<OrAnd, LeftRight, Type, BinaryPredicate,
				ListHead, ListTails..., Head, Tails...>::value;
		};
	}

	template<typename Type,
		typename ListHead, typename... ListTails>
		constexpr bool is_type_in_the_set_v = hide::type_opr_over_set_st<boolean::Or, boolean::Left,
		Type, std::is_same, ListHead, ListTails...>::value;

	template<typename Type,
		typename ListHead, typename... ListTails>
		constexpr bool is_constructible_over_the_set_v =
		hide::type_opr_over_set_st<boolean::Or, boolean::Right, Type, std::is_constructible, ListHead, ListTails...>::value;

	template<typename Type,
		typename ListHead, typename... ListTails>
		constexpr bool is_assignable_to_the_set_v =
		hide::type_opr_over_set_st<boolean::Or, boolean::Right,
		std::add_lvalue_reference_t<Type>, std::is_assignable, ListHead, ListTails...>::value;

	template<typename Type,
		typename ListHead, typename... ListTails>
		constexpr bool is_assignable_from_the_set_v =
		hide::type_opr_over_set_st<boolean::Or, boolean::Left,
		std::add_lvalue_reference_t<Type>, std::is_assignable, ListHead, ListTails...>::value;
		
	namespace hide
	{
		template<typename T>
		struct is_variant_st
		{
			static constexpr bool value = false;
		};

		template<typename T, typename... Types>
		struct is_variant_st<std::variant<T, Types...>>
		{
			static constexpr bool value = true;
		};
		
	} // end of namespace hide

	// forward declaration
	template<template<typename, typename...> class VariantType,
		typename VariantParamHead, typename... VariantParamTails,
		typename CallBackHead, typename... CallBackTails>
		bool process_variant(const VariantType<VariantParamHead, VariantParamTails...>& vart,
			CallBackHead&& call_back_head, CallBackTails&&... call_back_tails);

	template<typename T>
	constexpr bool is_variant_v = hide::is_variant_st<T>::value;
	
	template<typename ConversionClass,
		typename Head, typename... Tails>
		class variant_ext : public std::variant<Head, Tails...>
	{
	public:
		using elememt_types_t = tpf::type_list_t<Head, Tails...>;
		using base_t = std::variant<Head, Tails...>;
		static constexpr size_t element_count = sizeof...(Tails) + 1;

		using std::variant<Head, Tails...>::variant;

	public:
		variant_ext() : base_t{} {}
		variant_ext(const variant_ext&) = default;
		variant_ext& operator=(const variant_ext&) = default;

		const base_t& base() const
		{
			return static_cast<const base_t&>(*this);
		}

		base_t& base()
		{
			return static_cast<base_t&>(*this);
		}

		operator base_t&() { return static_cast<base_t&>(*this);  }

		template<typename TargetType> bool retrieve(TargetType& target_v)
		{
			auto process = [&target_v](auto& value, auto)
			{
				return ConversionClass::convert(target_v, value);
			};

			return tpf::process_variant(this->base(), process);
		}
	};

	using cstring_t =
		variant_ext<tpf::hide::StringConversion, const char*, const wchar_t*>;

	template<typename T>
	struct is_std_any_st
	{
		static constexpr bool value = false;
	};

	template<>
	struct is_std_any_st<std::any>
	{
		static constexpr bool value = true;
	};

	template<typename T>
	constexpr bool is_std_any_v = is_std_any_st<T>::value;

	template<typename T, typename R = void>
	using enable_if_std_any_t = std::enable_if_t<is_std_any_v<T>, R>;

	template<typename T, typename R = void>
	using enable_if_not_std_any_t = std::enable_if_t<!is_std_any_v<T>, R>;

	class bad_any_cast : public std::bad_any_cast
	{
	private:
		std::string m_msg;
	public:
		using base_t = std::bad_any_cast;
		using std::bad_any_cast::bad_any_cast;

		base_t& base() { return static_cast<base_t&>(*this); }
		const base_t& base() const { return static_cast<const base_t&>(*this); }

		bad_any_cast(std::string msg = "")
		{
			this->m_msg.append(this->base().what());

			if (msg.size() > 0) this->m_msg.append(msg);
		}

		virtual const char* what() const noexcept
		{
			return m_msg.c_str();
		}
	};
		
	template<size_t CURRENT, size_t MAX_COUNT, typename TypeAny>
	struct try_any_st
	{
		template<typename TryHead, typename... TryTrails>
			static size_t type_index(const TypeAny& any)
		{
			auto ptr = std::any_cast<TryHead>(&any.std());

			if (ptr) return CURRENT;
			else
			{
				if constexpr (CURRENT + 1 != MAX_COUNT)
					return try_any_st<CURRENT + 1, MAX_COUNT, TypeAny>:: 
					template type_index<TryTrails...>(any);
				else
					return size_t(-1);
			}
		}
	};

	template<size_t MAX_COUNT, typename TypeAny>
	struct try_any_st<MAX_COUNT, MAX_COUNT, TypeAny>
	{
		template<typename TryHead, typename... TryTrails>
			static size_t type_index(const TypeAny& any)
		{
			return size_t(-1);
		}
	};

	template<template<typename, typename...> typename TypeAny,
		typename Head, typename... Tails>
		size_t type_index(const TypeAny<Head, Tails...>& any)
	{
		constexpr size_t ELE_COUNT = sizeof...(Tails) + 1;
		return try_any_st<0, ELE_COUNT, TypeAny<Head, Tails...>>:: template
			type_index<Head, Tails...>(any);
	}
	   	 
	template<typename HeadType, typename... TailTypes>
	class any
	{
	public:
		static constexpr size_t type_count =
			sizeof...(TailTypes) + 1;

		using element_types_t = tpf::type_list_t<HeadType, TailTypes...>;
		using element_assign_types_t = 
			tpf::type_list_t<std::add_lvalue_reference_t<HeadType>, 
			std::add_lvalue_reference_t<TailTypes>...>;

	private:
		using any_t = std::any;

		any_t m_any;

	public:
		any() : m_any{} { }

		operator any_t&() { return this->m_any; }
		operator const any_t&() const { return this->m_any; }

		any_t& std() { return this->m_any; }
		const any_t& std() const { return this->m_any; }

		template<typename T>
		any(T v) : m_any{ static_cast<best_type_t<std::decay_t<T>, element_types_t>>(v) }
		{
			static_assert(best_type_v<std::decay_t<T>, element_types_t>,
				"The specified type is not allowed");
		}

		template<typename T> 
		any& operator=(const T& v)
		{
			using decayed_arg_t = std::decay_t<decltype(v)>;

			static_assert(tpf::is_assignable_to_the_set_v<decayed_arg_t, element_assign_types_t>,
				"The specified type is not allowed!");

			this->m_any = v; return *this;
		}

		template<typename TargetType> 
		std::enable_if_t<std::is_pointer_v<TargetType>, TargetType> get()
		{
			static_assert(tpf::is_type_in_the_set_v<
				std::remove_pointer_t<TargetType>, element_types_t>,
				"The specified type is not allowed!");
			using return_type_t = std::remove_pointer_t<TargetType>;

			// if fails, returns nullptr
			return std::any_cast<return_type_t>(&this->m_any);
		}

		template<typename TargetType>
		std::enable_if_t<!std::is_pointer_v<TargetType>, TargetType&> get();

		// {
		// 	static_assert(tpf::is_assignable_from_the_set_v<TargetType&, element_types_t>,
		// 		"The specified type is not allowed!");

		// 	TargetType* ptr = 
		// 		std::any_cast<TargetType>(&this->m_any);

		// 	if (ptr)
		// 	{
		// 		return *ptr;
		// 	}
		// 	else
		// 	{
		// 		// auto stream = tpf::stream();
				
		// 		auto stream = tpf::stream_t<char>{};

		// 		stream << "type case from " << this->m_any.type().name()
		// 			<< " to " << tpf::type_list_v<TargetType> << " failed\n";

		// 		throw tpf::bad_any_cast{ stream.str() };
		// 	}	
		// }

		template<typename TargetType> operator TargetType*()
		{
			static_assert(tpf::is_type_in_the_set_v<TargetType, element_types_t>,
			"The specified type is not allowed!");

			return std::any_cast<TargetType>(&this->m_any);
		}
			
		template<typename TargetType> operator TargetType&();
		// {
		// 	static_assert(tpf::is_assignable_from_the_set_v<TargetType&, element_types_t>,
		// 		"The specified type is not allowed");

		// 	auto ptr = std::any_cast<TargetType>(&this->m_any);

		// 	if (ptr)
		// 		return *ptr;
		// 	else
		// 	{
		// 		auto stream = tpf::stream();
		// 		stream << "type case from " << this->m_any.type().name()
		// 			<< " to " << tpf::type_list_v<TargetType> << " failed\n";

		// 		throw tpf::bad_any_cast{ stream.str() };
		// 	}
		// }

		size_t index()
		{
			return tpf::type_index(*this);
		}
		
	}; // end of constrained_any

	namespace hide
	{
		template<size_t INDEX, size_t ELEMENT_COUNT>
		struct st_static_lambda_loop
		{
			template<template<typename, typename...> class ContainerType,
				typename HeadType, typename... TailTypes,
				typename CallBackHead, typename... CallBackTails>
				static bool process_tuple_with_index( const ContainerType<HeadType, TailTypes...>& container,
					CallBackHead&& call_back_head, CallBackTails&&... call_back_tails)
			{
				
				if (!call_back_head(std::get<INDEX>(container), INDEX))
				{
					if constexpr (ELEMENT_COUNT != (INDEX + 1))
					{
						if constexpr (sizeof...(CallBackTails) == 0)
							return st_static_lambda_loop<INDEX + 1, ELEMENT_COUNT>::template
							process_tuple_with_index(container, std::forward<CallBackHead>(call_back_head));
						else
							return st_static_lambda_loop<INDEX + 1, ELEMENT_COUNT>::template
							process_tuple_with_index(container, std::forward<CallBackTails>(call_back_tails)...);
					}
					else
						return false;
				}
				else
					return true;
			}

			template<template<typename, typename...> class ContainerType,
				typename HeadType, typename... TailTypes,
				typename CallBackHead, typename... CallBackTails>
				static bool process_tuple_without_index(const ContainerType<HeadType, TailTypes...>& container,
					CallBackHead&& call_back_head, CallBackTails&&... call_back_tails)
			{

				if (!call_back_head(std::get<INDEX>(container)))
				{
					if constexpr (ELEMENT_COUNT != (INDEX + 1))
					{
						if constexpr (sizeof...(CallBackTails) == 0)
							return st_static_lambda_loop<INDEX + 1, ELEMENT_COUNT>::template
							process_tuple_without_index(container, std::forward<CallBackHead>(call_back_head));
						else
							return st_static_lambda_loop<INDEX + 1, ELEMENT_COUNT>::template
							process_tuple_without_index(container, std::forward<CallBackTails>(call_back_tails)...);
					}
					else
						return false;
				}
				else
					return true;
			}

			template<template<typename...> class ContainerType,
				typename... ContainerParameterTypes, typename Parameter,
				typename CallBackHead, typename... CallBacks>
				static bool call_func_ex(
					ContainerType<ContainerParameterTypes...>& container,
					Parameter&& param, CallBackHead&& call_back, CallBacks&&... call_backs)
			{
				// call operation
				if (!call_back(container, std::forward<Parameter>(param)))
				{
					if constexpr (ELEMENT_COUNT != (INDEX + 1))
					{
						if constexpr (sizeof...(call_backs) == 0)
						{
							return st_static_lambda_loop<INDEX + 1, ELEMENT_COUNT>::template
								call_func_ex(container, std::forward<Parameter>(param),
									std::forward<CallBackHead>(call_back));
						}
						else
						{
							return st_static_lambda_loop<INDEX + 1, ELEMENT_COUNT>::template
								call_func_ex(container, std::forward<Parameter>(param),
									std::forward<CallBacks>(call_backs)...);
						}
					}
					else
						return false;
				}
				else
					return true;
			}

			template<template<typename...> class ContainerType,
				typename... ContainerParameterTypes, typename CallBackHead, typename... CallBacks>
				static bool call_func(
					ContainerType<ContainerParameterTypes...>& container,
					CallBackHead&& call_back, CallBacks&&... call_backs)
			{
				// call operation
				if (!call_back(container))
				{
					if constexpr (ELEMENT_COUNT != (INDEX + 1))
					{
						if constexpr (sizeof...(call_backs) == 0)
						{
							return st_static_lambda_loop<INDEX + 1, ELEMENT_COUNT>::template
								call_func(container, std::forward<CallBackHead>(call_back));
						}
						else
						{
							return st_static_lambda_loop<INDEX + 1, ELEMENT_COUNT>::template
								call_func(container, std::forward<CallBacks>(call_backs)...);
						}
					}
					else
						return false;
				}
				else
					return true;
			}

			template<typename VariantType,
				typename CallBackHead, typename... CallBackTails>
			static bool process_variant_without_index(const VariantType& vart,
				CallBackHead&& call_back_head, CallBackTails&&... call_back_tails)
			{
				if (auto ptr = std::get_if<INDEX>(&vart))
				{
					call_back_head(*ptr);
					return true;
				}
				else
				{
					if constexpr (ELEMENT_COUNT != (INDEX + 1))
					{
						return st_static_lambda_loop<INDEX + 1, ELEMENT_COUNT>::template
							process_variant_without_index(vart, std::forward<CallBackTails>(call_back_tails)...);
					}
					else
						return false;
				}
			}

			template<typename VariantType,
				typename CallBackHead, typename... CallBackTails>
				static bool process_variant_with_index(const VariantType& vart,
					CallBackHead&& call_back_head, CallBackTails&&... call_back_tails)
			{
				if (auto ptr = std::get_if<INDEX>(&vart))
				{
					call_back_head(*ptr, INDEX);
					return true;
				}
				else
				{
					if constexpr (ELEMENT_COUNT != (INDEX + 1))
					{
						return st_static_lambda_loop<INDEX + 1, ELEMENT_COUNT>::template
							process_variant_with_index(vart, std::forward<CallBackHead>(call_back_head));
					}
					else
						return false;
				}
			}

			template<typename Head, typename... Tails,
				typename AnyType, typename CallBackHead, typename... CallBackTails>
				static bool process_any_without_index(const AnyType& any, 
					CallBackHead&& call_back_head, CallBackTails&&... call_back_tails)
			{
				auto ptr = std::any_cast<Head>(&any.std());
				
				if (ptr)
				{
					return call_back_head(*ptr);
				}
				else
				{
					if constexpr (ELEMENT_COUNT != (INDEX + 1))
					{
						return st_static_lambda_loop<INDEX + 1, ELEMENT_COUNT>::template
							 process_any_without_index<Tails...>(any, 
								 std::forward<CallBackTails>(call_back_tails)...);
					}
					else
						return false;
				}
			}

			template<typename Head, typename... Tails,
				typename AnyType, typename CallBackHead, typename... CallBackTails>
				static bool process_any_with_index(const AnyType& any,
					CallBackHead&& call_back_head, CallBackTails&&... call_back_tails)
			{
				auto ptr = std::any_cast<Head>(&any.std());

				if (ptr)
				{
					return call_back_head(*ptr, INDEX);
				}
				else
				{
					if constexpr (ELEMENT_COUNT != (INDEX + 1))
					{
						if constexpr(sizeof...(CallBackTails)==0)
							return st_static_lambda_loop<INDEX + 1, ELEMENT_COUNT>::template
								process_any_with_index<Tails...>(any, std::forward<CallBackHead>(call_back_head));
						else
							return st_static_lambda_loop<INDEX + 1, ELEMENT_COUNT>::template
								process_any_with_index<Tails...>(any, std::forward<CallBackTails>(call_back_tails)...);
					}
					else
						return false;
				}
			}
		};

		// dummy class
		template<size_t ELEMENT_COUNT>
		struct st_static_lambda_loop<ELEMENT_COUNT, ELEMENT_COUNT>
		{
			template<template<typename, typename...> class ContainerType,
				typename HeadType, typename... TailTypes,
				typename CallBackHead, typename... CallBackTails>
				static bool process_tuple_with_index(const ContainerType<HeadType, TailTypes...>& container,
					CallBackHead&& call_back_head, CallBackTails&&... call_back_tails)
			{
				return false;
			}

			template<template<typename, typename...> class ContainerType,
			typename HeadType, typename... TailTypes,
			typename CallBackHead, typename... CallBackTails>
			static bool process_tuple_without_index(const ContainerType<HeadType, TailTypes...>& container,
				CallBackHead&& call_back_head, CallBackTails&&... call_back_tails)
			{
				return false;
			}

			template<template<typename...> class ContainerType,
				typename... ContainerParameterTypes, typename Parameter>
				static bool call_func_ex(ContainerType<ContainerParameterTypes...>& container,
					Parameter&& param)
			{
				return false;
			}

			template<template<typename...> class ContainerType,
				typename... ContainerParameterTypes>
				static bool call_func(ContainerType<ContainerParameterTypes...>& container)
			{
				return false;
			}

			template<typename Head, typename... Tails,
			typename AnyType, typename CallBackHead, typename... CallBackTails>
			static bool process_any_with_index(const AnyType& any,
				CallBackHead&& call_back_head, CallBackTails&&... call_back_tails)
			{
				return false;
			}

			template<typename Head, typename... Tails,
			typename AnyType, typename CallBackHead, typename... CallBackTails>
			static bool process_any_without_index(const AnyType& any,
				CallBackHead&& call_back_head, CallBackTails&&... call_back_tails)
			{
				return false;
			}

			template<typename VariantType,
				typename CallBackHead, typename... CallBackTails>
				static bool process_variant_without_index(const VariantType& vart,
					CallBackHead&& call_back_head, CallBackTails&&... call_back_tails)
			{
				return false;
			}

			template<typename VariantType,
				typename CallBackHead, typename... CallBackTails>
				static bool process_variant_with_index(const VariantType& vart,
					CallBackHead&& call_back_head, CallBackTails&&... call_back_tails)
			{
				return false;
			}
		};

	} // end of namespace hide

	template<template<typename, typename...> class ContainerType,
		typename HeadType, typename... TailTypes,
		typename CallBackHead, typename... CallBackTails>
		bool process_tuple_elements(const ContainerType<HeadType, TailTypes...>& container,
		CallBackHead&& call_back_head, CallBackTails&&... call_back_tails)
	{
		if constexpr (sizeof...(TailTypes) == sizeof...(CallBackTails))
			return hide::st_static_lambda_loop<0, sizeof...(TailTypes)+1>
				::process_tuple_without_index(container,
					std::forward<CallBackHead>(call_back_head),
					std::forward<CallBackTails>(call_back_tails)...);
		else
			return hide::st_static_lambda_loop<0, sizeof...(TailTypes)+1>
				::process_tuple_with_index(container,
					std::forward<CallBackHead>(call_back_head),
					std::forward<CallBackTails>(call_back_tails)...);
	}


	template<template<typename...> class ContainerType,
		typename... ContainerParameterTypes, typename CallBackHead, typename... CallBacks>
		bool process_item(ContainerType<ContainerParameterTypes...>& container,
			CallBackHead&& call_back, CallBacks&&... call_backs)
	{
		if constexpr (sizeof...(CallBacks) == 0)
		{
			return hide::st_static_lambda_loop<0, sizeof...(ContainerParameterTypes)>
				::call_func(container, std::forward<CallBackHead>(call_back));
		}
		else
		{
			return hide::st_static_lambda_loop<0, sizeof...(ContainerParameterTypes)>
				::call_func(container,
					std::forward<CallBackHead>(call_back),
					std::forward<CallBacks>(call_backs)...);
		}
	}

	template<template<typename...> class ContainerType,
		typename... ContainerParameterTypes, typename Parameter,
		typename CallBackHead, typename... CallBacks>
		bool process_item_ex(ContainerType<ContainerParameterTypes...>& container,
			Parameter&& param, CallBackHead&& call_back, CallBacks&&... call_backs)
	{
		if constexpr (sizeof...(CallBacks) == 0)
		{
			return hide::st_static_lambda_loop<0, sizeof...(ContainerParameterTypes)>
				::call_func_ex(container, std::forward<Parameter>(param),
					std::forward<CallBackHead>(call_back));
		}
		else
		{
			return hide::st_static_lambda_loop<0, sizeof...(ContainerParameterTypes)>
				::call_func_ex(container, std::forward<Parameter>(param),
					std::forward<CallBackHead>(call_back),
					std::forward<CallBacks>(call_backs)...);
		}
	}

	template<template<typename, typename...> class VariantType,
		typename VariantParamHead, typename... VariantParamTails,
		typename CallBackHead, typename... CallBackTails>
		bool process_variant(const VariantType<VariantParamHead, VariantParamTails...>& vart,
			CallBackHead&& call_back_head, CallBackTails&&... call_back_tails)
	{
		if constexpr (sizeof...(call_back_tails) == sizeof...(VariantParamTails))
			return hide::st_static_lambda_loop<0, sizeof...(VariantParamTails) + 1>
				::process_variant_without_index(vart, std::forward<CallBackHead>(call_back_head),
					std::forward<CallBackTails>(call_back_tails)...);
		else
			return hide::st_static_lambda_loop<0, sizeof...(VariantParamTails) + 1>
			::process_variant_with_index(vart, std::forward<CallBackHead>(call_back_head),
				std::forward<CallBackTails>(call_back_tails)...);
	}

	template<template<typename, typename...> class AnyType, 
		typename Head, typename... Tails, typename CallBackHead, typename... CallBackTails>
		bool process_any(const AnyType<Head, Tails...>& any, 
			CallBackHead&& call_back_head, CallBackTails&&... call_back_tails)
	{
		if constexpr(sizeof...(call_back_tails) == sizeof...(Tails)+1)
			return hide::st_static_lambda_loop<0, sizeof...(Tails) + 1>:: template
			process_any_without_index<Head, Tails...>(any, 
				std::forward<CallBackHead>(call_back_head), 
				std::forward<CallBackTails>(call_back_tails)...);
		else
			return hide::st_static_lambda_loop<0, sizeof...(Tails) + 1>:: template
				process_any_with_index<Head, Tails...>(any, 
					std::forward<CallBackHead>(call_back_head), 
					std::forward<CallBackTails>(call_back_tails)...);
	}
	   
	namespace hide
	{
		template<typename T>
		struct is_character_ptr_st
		{
			static constexpr bool value = false;
		};

		template<>
		struct is_character_ptr_st<char*>
		{
			static constexpr bool value = true;
		};

		template<>
		struct is_character_ptr_st<wchar_t*>
		{
			static constexpr bool value = true;
		};

		template<>
		struct is_character_ptr_st<const char*>
		{
			static constexpr bool value = true;
		};

		template<>
		struct is_character_ptr_st<const wchar_t*>
		{
			static constexpr bool value = true;
		};
		
		template<typename T>
		struct is_constainer_st
		{
			static constexpr bool value = false;
		};

		template<typename T>
		struct is_constainer_st<std::vector<T>>
		{
			static constexpr bool value = true;
		};

		template<typename T>
		struct is_constainer_st<std::deque<T>>
		{
			static constexpr bool value = true;
		};

		template<typename T>
		struct is_constainer_st<std::set<T>>
		{
			static constexpr bool value = true;
		};

		template<typename T>
		struct is_constainer_st<std::multiset<T>>
		{
			static constexpr bool value = true;
		};

		template<typename T>
		struct is_constainer_st<std::list<T>>
		{
			static constexpr bool value = true;
		};

		template<typename Key, typename Value>
		struct is_constainer_st<std::map<Key, Value>>
		{
			static constexpr bool value = true;
		};

		template<typename Key, typename Value>
		struct is_constainer_st<std::multimap<Key, Value>>
		{
			static constexpr bool value = true;
		};

		template<typename Key>
		struct is_constainer_st<std::unordered_set<Key>>
		{
			static constexpr bool value = true;
		};

		template<typename Key>
		struct is_constainer_st<std::unordered_multiset<Key>>
		{
			static constexpr bool value = true;
		};

		template<typename Key, typename Value>
		struct is_constainer_st<std::unordered_map<Key, Value>>
		{
			static constexpr bool value = true;
		};

		template<typename Key, typename Value>
		struct is_constainer_st<std::unordered_multimap<Key, Value>>
		{
			static constexpr bool value = true;
		};

		template<typename T, size_t COUNT>
		struct is_constainer_st<std::array<T, COUNT>>
		{
			static constexpr bool value = true;
		};

		template<typename First, typename... Types>
		struct is_constainer_st<std::tuple<First, Types...>>
		{
			static constexpr bool value = true;
		};

		template<typename First, typename... Types>
		struct is_constainer_st<std::variant<First, Types...>>
		{
			static constexpr bool value = true;
		};

	} // end of namespace hide

	template<typename T>
	constexpr bool is_character_ptr_v = 
		hide::is_character_ptr_st<std::remove_reference_t<T>>::value;

	template<typename T, typename... Types>
	constexpr bool is_container_v = hide::is_constainer_st<T, Types...>::value;

	template<typename T, typename... Types>
	constexpr bool is_container_excluding_variant_v 
		= is_container_v<T, Types...> && !is_variant_v<T, Types...>;
	
	template<typename T, typename R = void>
	using enable_if_container_excluding_variant_t
		= std::enable_if_t<is_container_excluding_variant_v<T>, R>;

	template<typename T, typename R = void>
	using enable_if_container_t = std::enable_if_t<is_container_v<T>, R>;
		
	template<typename T>
	constexpr bool is_char_v =
		std::is_same_v<T, char> || std::is_same_v<T, unsigned char>;

	template<typename T>
	constexpr bool is_base_char_v = is_char_v<base_t<T>>;

	template<typename T>
	constexpr bool is_wchar_v = std::is_same_v<T, wchar_t>;

	template<typename T>
	constexpr bool is_base_wchar_v = is_wchar_v<base_t<T>>;

	template<typename T>
	constexpr bool is_character_v = is_char_v<T> || is_wchar_v<T>;

	template<typename T>
	constexpr bool is_base_character_v = is_character_v<base_t<T>>;

	template<typename T, typename R = void>
	using enable_if_char_t = std::enable_if_t<is_char_v<T>, R>;

	template<typename T, typename R = void>
	using enable_if_not_char_t = std::enable_if_t<!is_char_v<T>, R>;

	template<typename T, typename R = void>
	using enable_if_base_char_t = enable_if_char_t<base_t<T>, R>;

	template<typename T, typename R = void>
	using enable_if_not_base_char_t = enable_if_not_char_t<base_t<T>, R>;

	template<typename T, typename R = void>
	using enable_if_wchar_t = std::enable_if_t<is_wchar_v<T>, R>;

	template<typename T, typename R = void>
	using enable_if_not_wchar_t = std::enable_if_t<!is_wchar_v<T>, R>;

	template<typename T, typename R = void>
	using enable_if_base_wchar_t = enable_if_wchar_t<base_t<T>, R>;

	template<typename T, typename R = void>
	using enable_if_not_base_wchar_t = enable_if_not_wchar_t<base_t<T>, R>;

	template<typename T, typename R = void>
	using enable_if_character_t = std::enable_if_t<is_character_v<T>, R>;

	template<typename T, typename R = void>
	using enable_if_not_character_t = std::enable_if_t<!is_character_v<T>, R>;

	template<typename T, typename R = void>
	using enable_if_base_character_t = enable_if_character_t<base_t<T>, R>;

	template<typename T, typename R = void>
	using enable_if_not_base_character_t = enable_if_not_character_t<base_t<T>, R>;
	
	template<typename T>
	constexpr bool is_plain_type_v = !(std::is_class_v<T> || std::is_array_v<T>
		|| is_container_v<T> || is_base_character_v<T>);

	template<typename T, typename R = void>
	using enable_if_plain_type_t = std::enable_if_t<is_plain_type_v<T>, R>;
		
	namespace hide
	{
		template<typename CharType = char>
		struct complement_char_st
		{
			using type = wchar_t;
		};

		template<>
		struct complement_char_st<wchar_t>
		{
			using type = char;
		};

		template<typename StreamType = std::basic_ostringstream<char>>
		struct complement_stream_st
		{
			using type = std::basic_ostringstream<wchar_t>;
		};

		template<>
		struct complement_stream_st<std::basic_ostringstream<wchar_t>>
		{
			using type = std::basic_ostringstream<char>;
		};
	}

	template<typename CharType>
	using complement_char_t = typename hide::complement_char_st<CharType>::type;

	template<typename StreamType>
	using complement_stream_t = 
		typename hide::complement_stream_st<StreamType>::type;
		
	enum class stream_operation
	{
		// no operation
		no_operation = 0,
		// write buffer to console
		write_to_console,	 
		// write a new line to stream
		write_a_new_line,	 
		// flush stream buffer
		flush_stream_buffer, 
		// clear stream buffer
		clear_stream_buffer, 
		// empty stream buffer
		empty_stream_buffer, 
		// clear and empty 
		reset_stream_buffer  
	} str_opr_t;

	using stream_operations_t = 
		std::vector<stream_operation>;

	template<typename T, typename... Types>	
	stream_operations_t build_stream_operations(T&& first, Types&&... args)
	{
		return {std::forward<T>(first), std::forward<Types>(args)...};
	}

	namespace hide
	{
		template<typename CharType>
		class stream_out
		{
		public:
			
			// write stream buffer to console output
			static constexpr auto opr_console = stream_operation::write_to_console;
			// write a new line to the stream buffer
			static constexpr auto opr_newline = stream_operation::write_a_new_line;
			// flush the stream buffer
			static constexpr auto opr_flush = stream_operation::flush_stream_buffer;
			// clear the stream buffer's flags
			static constexpr auto opr_clear = stream_operation::clear_stream_buffer;
			// empty the stream buffer's contents
			static constexpr auto opr_empty = stream_operation::empty_stream_buffer;
			// clear the stream buffer's flags and empty the buffer 
			static constexpr auto opr_reset = stream_operation::reset_stream_buffer;

		public:
			
			using char_type = CharType;
			using complement_char_type = complement_char_t<char_type>;
			using stream_type = std::basic_stringstream<CharType>;
			using complement_stream_type = complement_stream_t<stream_type>;
			using string_type = std::basic_string<char_type>;
			using complement_string_type = std::basic_string<complement_char_type>;

			using manipulator_t = 
				decltype(std::endl<char_type, std::char_traits<char_type>>);
			
		private:
			template<typename T> using enable_if_base_char_t
				= std::enable_if_t<std::is_same_v<base_t<T>, char_type>, stream_out&>;

			template<typename T> using enable_if_complement_base_char_t
				= std::enable_if_t<std::is_same_v<base_t<T>, complement_char_type>, stream_out&>;
			
			template<typename T> using enable_if_plain_type_t 
				= tpf::enable_if_plain_type_t<T, stream_out&>;

			template<typename T>
				using enable_if_container_t = tpf::enable_if_container_t<T, stream_out&>;

			template<typename T> using enable_if_not_base_character_t
					= enable_if_not_base_character_t<T, stream_out&>;

			using string_convert_t =
				hide::string_conversion_st<char_type, complement_char_type>;

		private:
			template<size_t CURRENT, size_t MAX_ELEMENT>
			struct st_tuple_variant_print
			{
				template<typename...Types>
				static void out_tuple(const std::tuple<Types...>& v, stream_out& con_out)
				{
					con_out << std::get<CURRENT>(v);

					if (CURRENT + 1 < MAX_ELEMENT)
					{
						con_out.print_item_delimiter();
						st_tuple_variant_print<CURRENT + 1, MAX_ELEMENT>::out_tuple(v, con_out);
					}
				}

				template<typename T, typename...Types>
				static void out_variant(const std::variant<T, Types...>& v, stream_out& con_out)
				{
					if (CURRENT == v.index())
					{
						con_out << std::get<CURRENT>(v);
						return;
					}
					else
					{
						if (CURRENT + 1 < MAX_ELEMENT)
							st_tuple_variant_print<CURRENT + 1, MAX_ELEMENT>::out_variant(v, con_out);
					}
				}

			}; // end of primary struct st_tuple_variant_print

			template<size_t MAX_ELEMENT>
			struct st_tuple_variant_print<MAX_ELEMENT, MAX_ELEMENT>
			{
				template<typename...Types> static void
					out_tuple(const std::tuple<Types...>&, stream_out&) { 	}
				
				template<typename T, typename...Types> static void
					out_variant(const std::variant<T, Types...>&, stream_out&) { 	}

			}; // end of specialization struct st_tuple_variant_print

			template<typename... Types>
			stream_out& out_tuple(const std::tuple<Types...>& v, bool bEnclose = false)
			{
				if (bEnclose) this->print_enclose(true);

				st_tuple_variant_print<0, sizeof...(Types)>::out_tuple(v, *this);

				if (bEnclose) this->print_enclose(false);

				return *this;
			}

			template<typename T, typename... Types>
			stream_out& out_variant(const std::variant<T, Types...>& v)
			{
				st_tuple_variant_print<0, sizeof...(Types)+1>::out_variant(v, *this);

				return *this;
			}

			stream_out& out_type_list(const type_list_t<>& v, bool bEnclose = false)
			{
				if (bEnclose) this->print_angle(true);

				this->print_padding_space();

				if (bEnclose) this->print_angle(false);

				return*this;
			}

			template<typename T>
			stream_out& out_type_list(const type_list_t<T>& v, bool bEnclose = false)
			{
				if (bEnclose) this->print_angle(true);

				(*this) << type_to_string<T>();

				if (bEnclose) this->print_angle(false);

				return*this;
			}

			template<typename T, typename... Types>
			stream_out& out_type_list(const type_list_t<T, Types...>& v, bool bEnclose = false)
			{
				if (bEnclose) this->print_angle(true);

				(*this) << type_to_string<T>();

				if constexpr (sizeof...(Types) > 0)
				{
					this->print_item_delimiter();

					this->out_type_list(type_list_t<Types...>{}, false);
				}

				if (bEnclose) this->print_angle(false);

				return *this;
			}

			template<typename T> enable_if_container_t<T>
			out_container(const T& cntr, bool bEnclose = false)
			{
				if (cntr.empty())
				{
					if (bEnclose) this->print_enclose(true);

					this->print_padding_space();

					if (bEnclose) this->print_enclose(false);
				}
				else
				{
					if (bEnclose) this->print_enclose(true);

					auto last_element = cntr.cend();
					std::advance(last_element, -1);

					for (auto itr = cntr.cbegin(); itr != last_element;
						std::advance(itr, 1))
					{
						(*this) << *itr;
						this->print_item_delimiter();
					}
									   
					(*this) << *last_element;

					if (bEnclose) this->print_enclose(false);
				}

				return *this;
			}

		private:
			stream_type m_stream;
			string_type m_new_line;
			string_type m_empty_string;
			
			string_type m_enclose_open;
			string_type m_enclose_close;
			string_type m_angle_open;
			string_type m_angle_close;
			string_type m_item_delimiter;
			string_type m_padding_space;

			bool m_use_enclose;
			bool m_use_angle;
			bool m_use_line_break;
			bool m_use_item_delimiter;
			bool m_use_padding_space;

		public:
			
			stream_out& print_enclose(bool bOpen)
			{
				if (this->m_use_enclose)
				{
					if (bOpen)
					{
						if (!this->m_enclose_open.empty())
								(*this) << this->m_enclose_open;
					}
					else
					{
						if (!this->m_enclose_close.empty())
							(*this) << this->m_enclose_close;
					}
				}

				return *this;
			}

			stream_out& print_angle(bool bOpen)
			{
				if (this->m_use_angle)
				{
					if (bOpen)
					{
						if (!this->m_angle_open.empty())
							(*this) << this->m_angle_open;
					}
					else
					{
						if (!this->m_angle_close.empty())
							(*this) << this->m_angle_close;
					}
				}

				return *this;
			}

			stream_out& print_item_delimiter()
			{
				if (this->m_use_item_delimiter)
				{
					if (!this->m_item_delimiter.empty())
							(*this) << this->m_item_delimiter;
				}

				return *this;
			}

			stream_out& print_line_break()
			{
				if (this->m_use_line_break)
				{
					if (!this->m_new_line.empty())
						(*this) << this->m_new_line;
				}

				return *this;
			}

			stream_out& print_padding_space()
			{
				if (this->m_use_padding_space)
				{
					if (!this->m_padding_space.empty())
						(*this) << this->m_padding_space;
				}

				return *this;
			}
			
			stream_out(cstring_t enclose_open, cstring_t enclose_close,
				cstring_t item_delimiter, cstring_t angle_open,
				cstring_t angle_close, cstring_t padding_space) : m_stream{}
				{
					set_default_locale();

					enclose_open.retrieve(m_enclose_open);
					enclose_close.retrieve(m_enclose_close);
					item_delimiter.retrieve(m_item_delimiter);
					angle_open.retrieve(m_angle_open);
					angle_close.retrieve(m_angle_close);
					padding_space.retrieve(m_padding_space);
					
					this->m_new_line = stream_out::spacer("\n");
					this->m_empty_string = stream_out::spacer("");
					
					set_use_format();
				}
			
			stream_out& set_delimiters(
				cstring_t enclose_open, cstring_t enclose_close,
				cstring_t item_delimiter, cstring_t angle_open,
				cstring_t angle_close, cstring_t padding_space)
			{
				enclose_open.retrieve(m_enclose_open);
				enclose_close.retrieve(m_enclose_close);
				item_delimiter.retrieve(m_item_delimiter);
				angle_open.retrieve(m_angle_open);
				angle_close.retrieve(m_angle_close);
				padding_space.retrieve(m_padding_space);
				
				return *this;
			}

			stream_out(const stream_out&) = delete;
			stream_out& operator=(const stream_out&) = delete;

		public:
			
			stream_out& set_use_format(
				bool use_enclose = true,
				bool use_angle = true,
				bool use_line_break = true,
				bool use_item_delimiter = true,
				bool use_padding_space=true)
			{
				m_use_enclose = use_enclose;
				m_use_angle = use_angle;
				m_use_line_break = use_line_break;
				m_use_item_delimiter = use_item_delimiter;
				m_use_padding_space = use_padding_space;

				return *this;
			}

			stream_out& cout();

			operator std::ios_base&()
			{
				return this->m_stream;
			}

			decltype(auto) str() const
			{
				return this->m_stream.str();
			}

			decltype(auto) c_str() const
			{
				return this->m_stream.str().c_str();
			}

			operator const string_type&()
			{
				return this->m_stream.str();
			}

			operator const char_type*()
			{
				return this->m_stream.str().c_str();
			}
			
			static auto get_console_out()
			{
				return tpf::build_stream_operations(
					stream_out::opr_newline, 
					stream_out::opr_console, 
					stream_out::opr_reset);
			}

			const string_type& get_newline() const 
			{	return this->m_new_line; }

			const string_type& get_empty_string() const
			{	return this->m_empty_string; }

			const string_type& get_padding_space() const
			{	return this->m_padding_space; }
			
			const string_type& get_item_delimiter() const
			{
				return this->m_item_delimiter;
			}

			static auto spacer(char_type space_char, size_t count = 1)
			{
				string_type str(count, space_char);

				return str;
			}

			static auto spacer(complement_char_type space_char, size_t count = 1)
			{
				complement_string_type space_str(count, space_char);
				return string_convert_t::convert(space_str);
			}

			static auto spacer(const char_type* str)
			{
				return str;
			}

			static auto spacer(const complement_char_type* str)
			{
				return string_convert_t::convert(str);
			}

			stream_type& std() { return this->m_stream; }

			operator stream_type&() { return this->m_stream; }
			operator const stream_type&() const { return this->m_stream; }
			
			stream_out& operator<<(manipulator_t opr)
			{
				this->m_stream << opr;
				return *this;
			}

			template<typename ElementType>
			stream_out& operator<<(const std::reference_wrapper<ElementType>& rw)
			{
				(*this) << rw.get();
				return *this;
			}
			
			template<typename T, size_t ArrayCount> 
			enable_if_base_char_t<T>
				operator<<(const T(&v)[ArrayCount])
			{
				constexpr size_t LastElement = ArrayCount - 1;

				for (size_t i = 0; i < LastElement; ++i)
				{
					(*this)<< v[i];

					this->print_item_delimiter();
				}

				(*this) << v[LastElement];

				return *this;
			}

			template<typename T> enable_if_base_char_t<T>
				operator<<(const T& v)
			{
				this->m_stream << v; return *this;
			}

			template<typename First, typename Second>
			stream_out& operator<<(const std::pair<First, Second>& p)
			{
				this->print_enclose(true);

				(*this) << p.first;
				this->print_item_delimiter();
				(*this)<< p.second;

				this->print_enclose(false);
				
				return *this;
			}

			template<typename T>
			stream_out& operator()(T&& v)
			{
				(*this) << std::forward<T>(v);

				return *this;
			}

			template<typename T, typename... Types>
			stream_out& operator()(T&& first, Types&&... args)
			{
				(*this) << std::forward<T>(first);
				
				if (sizeof...(Types) > 0)
				{
					this->operator()(std::forward<Types>(args)...);
				}

				return *this;
			}

			template<typename T> enable_if_complement_base_char_t<T>
			operator<<(const T& v)
			{
				this->m_stream << string_convert_t::convert(v);
				return *this;
			}

			stream_out& operator << (const string_type& str)
			{
				this->m_stream << str; return *this;
			}

			stream_out& operator << (const complement_string_type& str)
			{
				this->m_stream << string_convert_t::convert(str);
				return *this;
			}

			template<typename T> enable_if_plain_type_t<T>
			operator<<(const T& v)
			{
				this->m_stream << v; return *this;
			}

			template<typename HeadType, typename... TailTypes>
			stream_out& operator<<(const any<HeadType, TailTypes...>& a)
			{
				auto& stream = *this;
				auto process = [&stream](const auto& value, auto)
				{
					stream << value; return true;
				};

				tpf::process_any(a, process);

				return *this;
			}
	
			stream_out& reset()
			{
				this->m_stream.clear();
				this->m_stream.str(m_empty_string);
				
				return *this;
			}

			stream_out& operator<<(const stream_operations_t& oprs)
			{
				for (auto& opr : oprs)
				{
					switch (opr)
					{
					case stream_operation::clear_stream_buffer:
						this->m_stream.clear(); break;

					case stream_operation::write_to_console:
						this->cout(); break;
					
					case stream_operation::write_a_new_line:
						(*this) << m_new_line; break;

					case stream_operation::empty_stream_buffer:
						this->m_stream.str(m_empty_string); break;

					case stream_operation::flush_stream_buffer:
						this->m_stream.flush(); break;

					case stream_operation::reset_stream_buffer:
						this->reset(); break;

					default: break;
					}
				}

				return *this;
			}

			template<typename T> enable_if_container_t<T>
			operator<<(const T& cntr)
			{
				return this->out_container(cntr, true);
			}

			template<typename First, typename... Types>
			stream_out& operator<<(const std::tuple<First, Types...>& v)
			{
				return this->out_tuple(v, true);
			}

			template<typename T, typename... Types>
			stream_out& operator<<(const std::variant<T, Types...>& v)
			{
				return this->out_variant(v);
			}

			stream_out& operator<<(const type_list_t<>& v)
			{
				return this->out_type_list(v, true);
			}

			template<typename T>
			stream_out& operator<<(const type_list_t<T>& v)
			{
				return this->out_type_list(v, true);
			}

			template<typename T, typename... Types>
			stream_out& operator<<(const type_list_t<T, Types...>& v)
			{
				return this->out_type_list(v, true);
			}

			// single dimensional array
			template<typename T, size_t ArrayCount>
			enable_if_not_base_character_t<T>
			operator<<(const T(&v)[ArrayCount])
			{
				constexpr size_t LastIndex = ArrayCount - 1;
				
				for (size_t i = 0; i < LastIndex; ++i)
				{
					(*this) << v[i]; this->print_item_delimiter();
				}
					
				(*this) << v[LastIndex];
				
				return *this;
			}
			
			template<size_t ArrayCount>
			stream_out& operator<<(const char_type(&v)[ArrayCount])
			{
				constexpr size_t LastIndex = ArrayCount - 1;
				
				// zero terminated string
				if (char_type{} == char_type(v[LastIndex]))
				{
					(*this) << decay_v(v);
				}
				else
				{
					for (size_t i = 0; i < LastIndex; ++i)
					{
						(*this) << v[i];
						this->print_item_delimiter();
					}

					(*this) << v[LastIndex];
				}

				return *this;
			}

			template<size_t ArrayCount>
			stream_out& operator<<(const complement_char_type(&v)[ArrayCount])
			{
				constexpr size_t LastIndex = ArrayCount - 1;
				
				auto start = decay_v(v); 

				// if zero-terminated, offset = 1
				// else offset = 0, pure array
				size_t offset = complement_char_type{} 
				== complement_char_type(v[LastIndex]) ? 1 : 0;
				
				complement_string_type src(start, start + ArrayCount - offset);
				
				string_type s = string_convert_t::convert(src);
				
				if (offset != 0) // zero-terminated string
				{
					(*this) << s; // print as normal string
				}
				else
				{	// print as array
					for (size_t i = 0; i < LastIndex; ++i)
					{
						(*this) << s[i];
						this->print_item_delimiter();
					}
					
					(*this) << s[LastIndex];
				}
				
				return *this;
			}
			
			template<typename T, typename... CountTypes>
			stream_out& out_array(const T* ele, CountTypes... args)
			{
				std::vector<size_t> bounds{ args... };
				constexpr size_t Ranks = sizeof...(args);

				std::vector<size_t> multipliers;
				multipliers.resize(Ranks);

				size_t mutiples = 1;

				for (size_t r = 1; r < Ranks; ++r)
				{
					multipliers[Ranks - r] = mutiples;
					mutiples *= bounds[Ranks - r];
				}
				multipliers[0] = mutiples;
				
				size_t Total = 1;
				for (auto& i : bounds) Total *= i;
				
				Total -= 1; // last element

				(*this) << ele[0];

				// first and last element excluded
				for (size_t i = 1; i < Total; ++i)
				{
					size_t newline = 0;
					for (size_t r = Ranks-1; r != 0; --r)
					{
						if (i % multipliers[r - 1] == size_t(0))
						{
							this->print_line_break();
							++newline;
						}
					}

					if (i%bounds[Ranks-1]!=size_t(0))
						this->print_item_delimiter();

					// print element
					(*this) << ele[i];
				}
				
				// last element
				if (Total != 0)
				{
					this->print_item_delimiter();
					(*this) << ele[Total];
				}
				
				return *this;
			}

			// two dimensional array
			template<typename T, size_t ArrayCount1, size_t ArrayCount2>
			stream_out& operator<<(const T(&v)[ArrayCount1][ArrayCount2])
			{
				return out_array(static_cast<const T*>(&v[0][0]),
					ArrayCount1, ArrayCount2);
			}

			template<typename T, size_t ArrayCount1,
				size_t ArrayCount2, size_t ArrayCount3>
				stream_out& operator<<(const T(&v)[ArrayCount1][ArrayCount2][ArrayCount3])
			{
				return out_array(static_cast<const T*>(&v[0][0][0]),
					ArrayCount1, ArrayCount2, ArrayCount3);
			}

			template<typename T, size_t ArrayCount1,
				size_t ArrayCount2, size_t ArrayCount3, size_t ArrayCount4>
				stream_out& operator<<(const T(&v)[ArrayCount1][ArrayCount2]
					[ArrayCount3][ArrayCount4])
			{
				return out_array(static_cast<const T*>(&v[0][0][0][0]),
					ArrayCount1, ArrayCount2, ArrayCount3, ArrayCount4);
			}

			template<typename T, size_t ArrayCount1,
				size_t ArrayCount2, size_t ArrayCount3, size_t ArrayCount4, size_t ArrayCount5>
				stream_out& operator<<(const T(&v)[ArrayCount1][ArrayCount2]
					[ArrayCount3][ArrayCount4][ArrayCount5])
			{
				return out_array(static_cast<const T*>(&v[0][0][0][0][0]),
					ArrayCount1, ArrayCount2, ArrayCount3, ArrayCount4, ArrayCount5);
			}
					   
			template<typename T, size_t ArrayCount1,
				size_t ArrayCount2, size_t ArrayCount3, size_t ArrayCount4,
				size_t ArrayCount5, size_t ArrayCount6>
				stream_out& operator<<(const T(&v)[ArrayCount1][ArrayCount2]
					[ArrayCount3][ArrayCount4][ArrayCount5][ArrayCount6])
			{
				return out_array(static_cast<const T*>(&v[0][0][0][0][0][0]),
					ArrayCount1, ArrayCount2, ArrayCount3, ArrayCount4,
					ArrayCount5, ArrayCount6);
			}

			template<typename T, size_t ArrayCount1,
				size_t ArrayCount2, size_t ArrayCount3, size_t ArrayCount4,
				size_t ArrayCount5, size_t ArrayCount6, size_t ArrayCount7>
				stream_out& operator<<(const T(&v)[ArrayCount1][ArrayCount2]
					[ArrayCount3][ArrayCount4][ArrayCount5][ArrayCount6][ArrayCount7])
			{
				return out_array(static_cast<const T*>(&v[0][0][0][0][0][0][0]),
					ArrayCount1, ArrayCount2, ArrayCount3, ArrayCount4,
					ArrayCount5, ArrayCount6, ArrayCount7);
			}

			template<typename T, size_t ArrayCount1,
				size_t ArrayCount2, size_t ArrayCount3, size_t ArrayCount4,
				size_t ArrayCount5, size_t ArrayCount6, size_t ArrayCount7,
				size_t ArrayCount8>
				stream_out& operator<<(const T(&v)[ArrayCount1][ArrayCount2]
					[ArrayCount3][ArrayCount4][ArrayCount5][ArrayCount6][ArrayCount7]
					[ArrayCount8])
			{
				return out_array(static_cast<const T*>(&v[0][0][0][0][0][0][0][0]),
					ArrayCount1, ArrayCount2, ArrayCount3, ArrayCount4,
					ArrayCount5, ArrayCount6, ArrayCount7, ArrayCount8);
			}

			template<typename T, size_t ArrayCount1,
				size_t ArrayCount2, size_t ArrayCount3, size_t ArrayCount4,
				size_t ArrayCount5, size_t ArrayCount6, size_t ArrayCount7,
				size_t ArrayCount8, size_t ArrayCount9>
				stream_out& operator<<(const T(&v)[ArrayCount1][ArrayCount2]
					[ArrayCount3][ArrayCount4][ArrayCount5][ArrayCount6]
					[ArrayCount7][ArrayCount8][ArrayCount9])
			{
				return out_array(static_cast<const T*>(&v[0][0][0][0][0][0][0][0][0]),
					ArrayCount1, ArrayCount2, ArrayCount3, ArrayCount4, ArrayCount5,
					ArrayCount6, ArrayCount7, ArrayCount8, ArrayCount9);
			}

			template<typename T, size_t ArrayCount1,
				size_t ArrayCount2, size_t ArrayCount3, size_t ArrayCount4,
				size_t ArrayCount5, size_t ArrayCount6, size_t ArrayCount7,
				size_t ArrayCount8, size_t ArrayCount9, size_t ArrayCount10>
				stream_out& operator<<(const T(&v)[ArrayCount1][ArrayCount2]
					[ArrayCount3][ArrayCount4][ArrayCount5][ArrayCount6]
					[ArrayCount7][ArrayCount8][ArrayCount9][ArrayCount10])
			{
				return out_array(static_cast<const T*>(&v[0][0][0][0][0][0][0][0][0][0]),
					ArrayCount1, ArrayCount2, ArrayCount3, ArrayCount4,	ArrayCount5,
					ArrayCount6, ArrayCount7, ArrayCount8, ArrayCount9, ArrayCount10);
			}

		};
				
		template<typename CharType>
		stream_out<CharType>& stream_out<CharType>::cout()
		{
			if constexpr (std::is_same_v<CharType, char>)
				std::cout << this->m_stream.str();
			else
				std::wcout << this->m_stream.str();
		    
			return *this;
		}
	} // end of namespace hide
	
	template<typename CharType = wchar_t>
	using stream_t = hide::stream_out<CharType>;

	template<typename HeadType, typename... TailTypes>
	template<typename TargetType>
		std::enable_if_t<!std::is_pointer_v<TargetType>, TargetType&> any<HeadType, TailTypes...>::get()
		{
			static_assert(tpf::is_assignable_from_the_set_v<TargetType&, element_types_t>,
				"The specified type is not allowed!");

			TargetType* ptr = 
				std::any_cast<TargetType>(&this->m_any);

			if (ptr)
			{
				return *ptr;
			}
			else
			{
				// auto stream = tpf::stream();
				
				auto stream = tpf::stream_t<char>{};

				stream << "type case from " << this->m_any.type().name()
					<< " to " << tpf::type_list_v<TargetType> << " failed\n";

				throw tpf::bad_any_cast{ stream.str() };
			}	
		}

		template<typename HeadType, typename... TailTypes>
		template<typename TargetType> any<HeadType, TailTypes...>::operator TargetType&()
		{
			static_assert(tpf::is_assignable_from_the_set_v<TargetType&, element_types_t>,
				"The specified type is not allowed");

			auto ptr = std::any_cast<TargetType>(&this->m_any);

			if (ptr)
				return *ptr;
			else
			{
				// auto stream = tpf::stream();
				auto stream = tpf::stream_t<char>{};

				stream << "type case from " << this->m_any.type().name()
					<< " to " << tpf::type_list_v<TargetType> << " failed\n";

				throw tpf::bad_any_cast{ stream.str() };
			}
		}


	inline decltype(auto) stream(cstring_t enclose_open = "{",
		cstring_t enclose_close = "}",
		cstring_t item_delimiter = ", ",
		cstring_t angle_open = "<",
		cstring_t angle_close = ">",
		cstring_t padding_space = " ")
	{
		return stream_t<char>{enclose_open, enclose_close,
			item_delimiter, angle_open, angle_close, padding_space};
	}

	inline decltype(auto) wstream(
		cstring_t enclose_open = L"{",
		cstring_t enclose_close = L"}",
		cstring_t item_delimiter = L", ",
		cstring_t angle_open = L"<",
		cstring_t angle_close = L">",
		cstring_t padding_space = L" ")
	{
		return stream_t<wchar_t>{enclose_open, enclose_close,
			item_delimiter, angle_open, angle_close, padding_space};
	}
} // end of namespace tpf

#ifdef _TPF_MIN_DEFINED
#pragma pop_macro("min")
#undef _TPF_MIN_DEFINED
#endif 

#ifdef _TPF_MAX_DEFINED
#pragma pop_macro("max")
#undef _TPF_MAX_DEFINED
#endif 

#endif // end of _TPF_TYPE_UTIL_H
