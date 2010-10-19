direc = File.dirname(__FILE__)

require "rbconfig"
require "#{direc}/mult/version"

direc = File.dirname(__FILE__)
dlext = Config::CONFIG['DLEXT']
begin
    if RUBY_VERSION && RUBY_VERSION =~ /1.9/
        require "#{direc}/1.9/mult.#{dlext}"
    else
        require "#{direc}/1.8/mult.#{dlext}"
    end
rescue LoadError => e
    require "#{direc}/mult.#{dlext}"
end
