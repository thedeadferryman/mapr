require "pathname"
require_relative "commons"

# region args

def help
  STDERR.puts "Usage: #{__FILE__} <ROOT_DIR> <PROJECT_NAME> [RUN_MODE]"
  STDERR.puts "\tROOT_DIR                : source resolution root"
  STDERR.puts "\tPROJECT_NAME            : project name as stated in include guards"
  STDERR.puts "\tRUN_MODE=check (default): find problems, do not fix, exit normally"
  STDERR.puts "\tRUN_MODE=strict         : find problems, do not fix, exit fail if any"
  STDERR.puts "\tRUN_MODE=fix            : find problems and fix"

  exit 1
end

if ARGV.size < 2
  help
end

ROOT_DIR = File.realpath ARGV.shift # arg0 - source resolution root
PROJECT_NAME = ARGV.shift # arg1 - run mode
RUN_MODE = ARGV.shift || "check" # arg1 - run mode

ensure_mode(RUN_MODE) { help }

# endregion args

messages = []

Dir["#{ROOT_DIR}/**/*.{#{SOURCE_EXTS}}"].each do |ent|
  extname = File.extname(ent)
  basename = File.basename(ent, extname)
  extname = extname[1..-1]

  fname = Pathname.new(ent).relative_path_from(ROOT_DIR).to_s

  next unless extname.start_with?("h")

  guard_slug = [PROJECT_NAME, basename, extname].map(&:upcase).join("_")
  guard_regex = %r{#ifndef[\s]+#{guard_slug}[\n\r\s]+#define[\s]+#{guard_slug}}
  end_guard_regex = %r{#endif[\s]+\/\/[\s]+#{guard_slug}}

  contents = IO.read(ent)

  match = contents.scan(guard_regex).first&.gsub(%r{[\r\n]}, ' \ ')

  messages << %(In #{fname}: "#{match}" -> "#pragma once") unless match.nil?

  new_contents = contents
    .gsub(guard_regex, "#pragma once")
    .gsub(end_guard_regex, "")

  if RUN_MODE == "fix"
    IO.write(ent, new_contents)
  end
end

finish_script(messages, RUN_MODE)