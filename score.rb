require 'json'

WorkDir = ARGV[0] || 'solutions/mnk'
SummaryDir = File.join(WorkDir, 'summary')

class Score
  def initialize(sol, result)
    @sol = sol
    if String === result
      @moves = Float::INFINITY
      @time = 6
      @desc = result
      @score = 0
    else
      @time = result["time"]
      @moves = result["moves"]
      @desc = "#{@moves} moves in #{(@time*1000).round} ms"
    end
  end

  attr_reader :sol, :time, :moves, :desc
  attr_accessor :score
end

def total(test_results)
  ret = Hash.new(0)
  test_results.each do |test, scores|
    scores.each do |score|
      ret[score.sol] += score.score
    end
  end
  ret.to_a.sort_by { |sol, score| score }.reverse
end

def score_shortest(summary)
  summary["report"].transform_values { |test_data|
    test_result = test_data.each.map { |sol, result|
      score = Score.new(sol, result)
    }.group_by { |score|
      score.moves
    }.map { |moves, scores|
      [moves, scores.sort_by { |s| s.time }]
    }.sort_by { |g|
      g[0]
    }

    next test_result if !(Integer === test_result[0][0]) # abort scoring if all have 0 points for the test result

    additional_points = 3
    test_result[0][1].each do |score|
      score.score = 10 + additional_points
      additional_points -= 1 if additional_points != 0
    end

    test_result[1..].each do |moves, scores|
      break if !(Integer === moves)
      scores.each do |score|
        score.score = 1
      end
    end

    test_result.each_with_index do |tr, i|
    end

    test_result.map { |moves|
      moves[1]
    }.flatten
  }
end

def tr(*ar)
  '|' + ar.join('|') + '|'
end

def write_tables(f, slinks, test_results)
  f.puts
  f.puts tr('#', 'Solution', 'Score')
  f.puts tr(['---']*3)

  total(test_results).each_with_index do |ss, i|
    f.puts tr(i+1, slinks[ss[0]], ss[1])
  end

  test_results.each do |test, scores|
    f.puts
    f.puts "## #{test}"
    f.puts
    f.puts tr('#', 'Solution', 'Result', 'Score')
    f.puts tr(['---']*4)
    scores.each_with_index do |score, i|
      f.puts tr(i+1, slinks[score.sol], score.desc, score.score)
    end
  end
end

summary = JSON.parse(File.read(File.join(SummaryDir, 'summary.json')))

slinks = summary['solutions'].map { |name, src|
  [name, "[#{name}](../#{File.basename(src)})"]
}.to_h

File.open(File.join(SummaryDir, "shortest.md"), 'w') do |f|
  f.puts '# Shortest'
  write_tables(f, slinks, score_shortest(summary))
end
