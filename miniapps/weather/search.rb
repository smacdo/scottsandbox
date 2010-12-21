#!/usr/bin/ruby1.9
require 'rubygems'
require 'hpricot'
require 'net/http'

path = "http://api.wunderground.com/auto/wui/geo/GeoLookupXML/index.xml?query="
loc = "60607"

woid = "12784261"
url = 

url = URI.parse( path + loc )

puts url

begin
	resp, data = Net::HTTP.get( url )
	#puts "RESPONSE: #{resp}"
	#puts "DATA    : #{data}"
rescue
	puts "Connection error: #{$!}"
end
