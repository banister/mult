direc = File.dirname(__FILE__)

require 'rake/clean'
require 'rake/gempackagetask'
require 'rake/rdoctask'
require "#{direc}/lib/mult/version"

dlext = Config::CONFIG['DLEXT']

CLEAN.include("ext/**/*.#{dlext}", "ext/**/.log", "ext/**/.o", "ext/**/*~", "ext/**/*#*", "ext/**/.obj", "ext/**/.def", "ext/**/.pdb")
CLOBBER.include("**/*.#{dlext}", "**/*~", "**/*#*", "**/*.log", "**/*.o", "doc/**")

spec = Gem::Specification.new do |s|
  s.name = "mult"
  s.summary = "a few handy extensions to mess with ruby objects"
  s.description = s.summary
  s.version = Mult::VERSION
  s.author = "John Mair (banisterfiend)"
  s.email = 'jrmair@gmail.com'
  s.date = Time.now.strftime '%Y-%m-%d'
  s.require_path = 'lib'
  s.homepage = "http://banisterfiend.wordpress.com"
  s.platform = 'i386-mswin32'
  #s.platform = Gem::Platform::RUBY
  #s.extensions = FileList["ext/**/extconf.rb"]
  s.has_rdoc = 'yard'
  s.files = ["Rakefile", "README.rdoc", "LICENSE", "lib/mult.rb", "lib/mult/version.rb"] +
    ["lib/1.8/mult.#{dlext}", "lib/1.9/mult.#{dlext}"] +
    FileList["ext/**/extconf.rb", "ext/**/*.h", "ext/**/*.c"].to_a
end

Rake::GemPackageTask.new(spec) do |pkg|
  pkg.need_zip = false
  pkg.need_tar = false
end

# Rake::ExtensionTask.new('mult', spec)  do |ext|
#     ext.config_script = 'extconf.rb' 
#     ext.cross_compile = true                
#     ext.cross_platform = 'i386-mswin32'
# end

Rake::RDocTask.new do |rd|
  rd.main = "README.rdoc"
  rd.rdoc_files.include("README.rdoc", "ext/mult/*.c")
end

