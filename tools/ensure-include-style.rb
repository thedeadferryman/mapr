#!/usr/bin/env ruby

require "pathname"
require_relative "commons"

INC_REGEX = %r{#include[\s]*["<](.*)[">]}

# region args

def help
  STDERR.puts "Usage: #{__FILE__} <ROOT_DIR> [RUN_MODE]"
  STDERR.puts "\tROOT_DIR                : source resolution root"
  STDERR.puts "\tRUN_MODE=check (default): find problems, do not fix, exit normally"
  STDERR.puts "\tRUN_MODE=strict         : find problems, do not fix, exit fail if any"
  STDERR.puts "\tRUN_MODE=fix            : find problems and fix"

  exit 1
end

if ARGV.size < 1
  help
end

ROOT_DIR = File.realpath ARGV.shift # arg0 - source resolution root
RUN_MODE = ARGV.shift || "check" # arg1 - run mode

ensure_mode(RUN_MODE) { help }

# endregion args

STDERR.puts "RUN_MODE=#{RUN_MODE}"

messages = []

Dir["#{ROOT_DIR}/**/*.{#{SOURCE_EXTS}}"].each do |ent|
  ent = File.realpath(ent)

  contents = IO.read(ent)

  entity = File.basename(ent, File.extname(ent))
  entity_loc = File.dirname(ent)

  newc = contents.gsub(INC_REGEX) do |match|
    from_current = File.expand_path($1, File.dirname(ent))

    root_inc = if File.exists?(from_current) # file is resolved relatively
        Pathname.new(from_current).relative_path_from(ROOT_DIR)
      else
        $1
      end

    full_inc = File.expand_path(root_inc, ROOT_DIR)

    real_inc = if File.exists? full_inc # file belongs to project
        inc_entity = File.basename(full_inc, File.extname(full_inc))
        inc_entity_loc = File.dirname(full_inc)

        if entity == inc_entity and entity_loc == inc_entity_loc # file is a "main include", include relatively
          %(#include "#{File.basename(full_inc)}")
        else # file belongs to project, include by source root
          %(#include "#{root_inc}")
        end
      else # file is external, include as is
        %(#include <#{root_inc}>)
      end

    file_rel = Pathname.new(ent).relative_path_from(ROOT_DIR)

    if match != real_inc
      messages << %(In #{file_rel}: "#{match}" -> "#{real_inc}")
    end

    real_inc
  end

  if RUN_MODE == "fix"
    IO.write(ent, newc)
  end
end


finish_script(messages, RUN_MODE)