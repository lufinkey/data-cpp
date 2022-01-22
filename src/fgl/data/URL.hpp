//
//  URL.hpp
//  DataCpp
//
//  Created by Luis Finke on 1/4/22.
//  Copyright © 2022 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/data/String.hpp>
#include <fgl/data/ArrayList.hpp>
#include <fgl/data/Map.hpp>
#include <fgl/data/Optional.hpp>

namespace fgl {
	class URL {
	public:
		/// A (decoded) key-value item for the query component of a URL
		struct QueryItem {
			/// The (decoded) key of the query item
			String key;
			/// The (decoded) value of the query item
			Optional<String> value;
			
			static QueryItem parse(const char* begin, const char* end);
			static QueryItem parse(const std::string& str);
			String toString() const;
		};
		
		/// Contains the individual components of a URL
		struct Components {
			explicit Components();
			explicit Components(const std::string&);
			
			static Optional<Components> parse(const std::string&);
			
			/// The scheme of the URL (ex: ftp, http, https)
			Optional<String> scheme;
			/// The username component of the URL
			Optional<String> username;
			/// The password component of the URL
			Optional<String> password;
			/// The host component of the URL
			Optional<String> host;
			/// The port component of the URL
			Optional<uint16_t> port;
			/// The encoded path portion of the URL
			String path;
			/// The encoded URL query component, if available
			Optional<String> query;
			/// The URL fragment (hash) component, if available
			Optional<String> fragment;
			
			/// The decoded items in the query component
			LinkedList<QueryItem> queryItems() const;
			/// Sets the query items for the query string
			void setQueryItems(const ArrayList<QueryItem>& queryItems);
			
			/// Tells whether the URL path is an absolute or relative path
			bool pathIsRelative() const;
			/// Tells whether the URL path has a trailing slash
			bool pathHasTrailingSlash() const;
			/// Controls whether or not the path component should have a trailing slash
			void setPathHasTrailingSlash(bool hasTrailingSlash);
			/// The individual decoded parts of the URL path, split by the '/' delimiter
			LinkedList<String> pathParts() const;
			/// Sets the URL path from its decoded parts. If relative is `true`, a leading slash will not be included in the final path
			void setPathParts(const ArrayList<String>& parts, bool relative = false);
		};
		
		static String makeQueryString(const ArrayList<QueryItem>& items);
		static String makeQueryString(const std::map<String,String>& items);
		static LinkedList<QueryItem> parseQueryItems(const String& queryString);
		static Map<String,String> parseQueryItemMap(const String& queryString);
		
		static String encodeUserComponent(const std::string& str);
		static String decodeUserComponent(const char* begin, const char* end);
		static String decodeUserComponent(const std::string& str);
		
		static String encodePathPart(const std::string& str);
		static String decodePathPart(const char* begin, const char* end);
		static String decodePathPart(const std::string& str);
		
		static String encodeQueryKey(const std::string& str);
		static String decodeQueryKey(const char* begin, const char* end);
		static String decodeQueryKey(const std::string& str);
		
		static String encodeQueryValue(const std::string& str);
		static String decodeQueryValue(const char* begin, const char* end);
		static String decodeQueryValue(const std::string& str);
		
		static bool isSchemeValid(const char* begin, const char* end);
		static bool isSchemeValid(const std::string&);
		static bool isHostValid(const char* begin, const char* end);
		static bool isHostValid(const std::string&);
		static bool isUsernameValid(const char* begin, const char* end);
		static bool isUsernameValid(const std::string&);
		static bool isPasswordValid(const char* begin, const char* end);
		static bool isPasswordValid(const std::string&);
		static bool isPortValid(const char* begin, const char* end);
		static bool isPortValid(const std::string&);
		static bool isPathValid(const char* begin, const char* end);
		static bool isPathValid(const std::string&);
		static bool isQueryValid(const char* begin, const char* end);
		static bool isQueryValid(const std::string&);
		static bool isFragmentValid(const char* begin, const char* end);
		static bool isFragmentValid(const std::string&);
		
		static Optional<URL> parse(const std::string& str);
		
		explicit URL(const std::string&);
		explicit URL(Components);
		String toString() const;
		
		const Optional<String>& scheme() const;
		const Optional<String>& username() const;
		const Optional<String>& password() const;
		const Optional<String>& host() const;
		const Optional<uint16_t>& port() const;
		const String& path() const;
		const Optional<String>& query() const;
		const Optional<String>& fragment() const;
		
		bool pathIsRelative() const;
		bool pathHasTrailingSlash() const;
		LinkedList<String> pathParts() const;
		
		LinkedList<QueryItem> queryItems() const;
		void setQueryItems(const ArrayList<QueryItem>& queryItems);
		
		const Components& components() const;
		
	private:
		static String encodeComponent(const std::string& str, Function<bool(char)> shouldEncodeChar, const std::map<char,char>& mappedChars = {});
		static String decodeComponent(const char* begin, const char* end, const std::map<char,char>& mappedChars = {});
		
		/// The individual components of the URL
		Components _components;
	};
}
