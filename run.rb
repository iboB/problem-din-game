require 'timeout'
require 'fileutils'
require 'json'

WorkDir = ARGV[0] || 'solutions/mnk'
BuildDir = File.join(WorkDir, 'build')
ResultDir = File.join(WorkDir, 'results')

FileUtils.mkdir_p BuildDir
FileUtils.mkdir_p ResultDir

def build_cxx(name, source)
  exe = File.join(BuildDir, name)
  cmdline = "g++ #{source} -std=c++17 -O3 -o #{exe}"
  # `#{cmdline}`
  {name: name, exe: exe}
end

def build(file)
  name = File.basename(file, '.*')
  puts "Building solution '#{name}'"
  case File.extname(file)
  when '.cpp'
    build_cxx(name, file)
  else
    puts "Error: No rule available to build '#{file}'"
  end
end

puts 'Building solutions...'
exes = Dir[File.join(WorkDir, '*')].map { |file|
  next if !File.file?(file)
  build(file)
}.compact

def execute(exe, input_file, output_file, time_limit)
  pid = Process.spawn(exe, :in=>input_file, :out=>output_file)
  start = Time.new.to_f
  begin
    Timeout::timeout(time_limit) { Process.wait pid }
  rescue Timeout::Error
    Process.kill 9, pid
    return time_limit + 1
  end
  Time.new.to_f - start
end

def check(input, output)
  c = input.dup
  output.each do |s, t|
    c[s] -= c[t]
    c[t] *= 2
  end
  c.include? 0
end

puts 'Running tests...'
report = Dir['tests/*'].map { |input_file|
  input = File.open(input_file, &:readline).split(' ').map(&:to_i)
  tname = File.basename(input_file)

  puts "#{tname}: #{input}"

  result = exes.map { |edata|
    ename = edata[:name]
    print "#{ename}: "

    output_file = File.join(ResultDir, ename + '.' + tname)

    time = (0..3).map { execute(edata[:exe], input_file, output_file, 5) }.min

    if time == 6
      puts 'timeout'
      next [ename, 'timeout']
    end

    print "#{time} seconds, "

    output = File.readlines(output_file).map { |move|
      break [] if (move =~ /([A-C])->([A-C])/) != 0
      [$1.ord - 65, $2.ord - 65]
    }

    if !check(input, output)
      puts "incorrect"
      [ename, 'incorrect']
    else
      puts "correct in #{output.length} moves"
      [ename, {time: time, moves: output.length}]
    end
  }
  [input.join(' '), result.to_h]
}.to_h

File.write(File.join(ResultDir, 'report.json'), JSON.pretty_generate(report))
