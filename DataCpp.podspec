#
# Be sure to run `pod lib lint DataCpp.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see https://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
	s.name             = 'DataCpp'
	s.version          = '0.1.0'
	s.summary          = 'A small data library for C++.'

# This description is used to generate tags and improve search results.
#   * Think: What does it do? Why did you write it? What is the focus?
#   * Try to keep it short, snappy and to the point.
#   * Write the description between the DESC delimiters below.
#   * Finally, don't worry about the indent, CocoaPods strips it!

	s.description      = <<-DESC
	A small data library for C++.
	DESC

	s.homepage         = 'https://github.com/lufinkey/data-cpp'
	# s.screenshots     = 'www.example.com/screenshots_1', 'www.example.com/screenshots_2'
	# s.license          = { :type => 'MIT', :file => 'LICENSE' }
	s.author           = { 'Luis Finke' => 'luisfinke@gmail.com' }
	s.source           = { :git => 'https://github.com/lufinkey/data-cpp.git', :tag => s.version.to_s }
	s.social_media_url = 'https://twitter.com/lufinkey'

	s.ios.deployment_target = '12.0'
	s.osx.deployment_target = '10.14'

	s.source_files = 'src/fgl/**/*.{c,cpp,cc,m,mm,h,hpp,inl,impl}'
  
	# s.resource_bundles = {
	#   'DataCpp' => ['DataCpp/Assets/*.png']
	# }

	s.public_header_files = 'src/fgl/**/*.hpp'
	s.header_mappings_dir = 'src/fgl'
	s.pod_target_xcconfig = {
		'HEADER_SEARCH_PATHS' => "$(PODS_ROOT)/DataCpp/src",
		'CLANG_CXX_LANGUAGE_STANDARD' => 'gnu++20',
		'GCC_OPTIMIZATION_LEVEL[config=Release]' => 's'
	}
	s.user_target_xcconfig = {
		'FRAMEWORK_SEARCH_PATHS' => "${PODS_ROOT}/DataCpp/src"
	}
	# s.frameworks = 'UIKit', 'MapKit'
	# s.dependency 'AFNetworking', '~> 2.3'
end
