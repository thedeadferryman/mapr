SOURCE_EXTS = %w[c cc cpp cxx h hh hpp hxx].join(",")
ALLOWED_MODES = %w[check strict fix]

module Enumerable
  def pluck(id)
    self.map { |item| item.respond_to?(:[]) ? item[id] : nil }
  end
end

def ensure_mode(mode, &blk)
  unless ALLOWED_MODES.include? mode
    STDERR.puts "RUN_MODE invalid, expected one of #{ALLOWED_MODES.join(", ")}"
    blk.call
    exit 1
  end
end

def finish_script(messages, mode)
  STDERR.puts messages.join("\n")

  case mode
  when %r{(check|strict)}
    if messages.empty?
      STDERR.puts "no problems"
    else
      STDERR.puts "found #{messages.size} problem(s)"

      exit -1 if mode == "strict"
    end
  when "fix"
    STDERR.puts "fixed #{messages.size} problem(s)"
  end
end
