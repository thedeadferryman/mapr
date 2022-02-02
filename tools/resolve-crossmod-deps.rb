#/usr/bin/env ruby

require "pathname"
require "json"
require_relative "commons"

ROOT_DIR = File.realpath ARGV.shift # arg0 - source resolution root
REPORT_FILE = ARGV.shift

raise 'Root dir not specified!' if ROOT_DIR.nil?
raise 'Report destination not specified!' if REPORT_FILE.nil?

modules = []

Dir["#{ROOT_DIR}/*"].each do |ent|
  next unless File.directory? ent

  mod = {
    name: File.basename(ent),
    path: ent
  }

  modules << mod
end

modnames = modules.pluck(:name).join('|')

inc_regex = %r{\#include[\s]*\<((#{modnames})\/.*)\>}

modules = modules.map do |mod|
  depends_on = []
  dep_map = {}

  Dir["#{mod[:path]}/**/*.{#{SOURCE_EXTS}}"].each do |file|
    next if File.directory? file

    src = IO.read(file)

    rel_file = Pathname.new(file).relative_path_from(ROOT_DIR).to_s

    results = src.scan inc_regex

    depends_on += results.pluck(1)
    dep_map[rel_file] = results.pluck(0)
  end

  {
    **mod,
    depends_on: depends_on.uniq - [mod[:name]],
    dependency_map: dep_map
  }
end

report = JSON.pretty_generate({
  location: ROOT_DIR,
  modules: modules
})

IO.write REPORT_FILE, report