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
      @desc = "#{@moves} / #{(@time*1000).round} ms"
    end
  end

  attr_reader :sol, :time, :moves, :desc
  attr_accessor :rank, :score
end

class SolutionScore
  def initialize
    @total = 0
    @scores = {}
  end
  attr_accessor :scores, :total
end

def per_solution(test_results)
  ret = {}
  test_results.each do |test, scores|
    scores.each do |score|
      ret[score.sol] = SolutionScore.new if !ret[score.sol]
      ss = ret[score.sol]
      ss.total += score.score
      ss.scores[test] = score
    end
  end
  ret.to_a.sort_by { |s, ss| ss.total }.reverse
end

def score_shortest(summary)
  per_solution summary["report"].transform_values { |test_data|
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
    }.flatten.each_with_index do |score, i|
      score.rank = i+1
    end
  }
end

def tr(ar)
  '|' + ar.join('|') + '|'
end

def gen_table(summary, scores_per_solution)
  solutions = summary["solutions"]
  report = summary["report"]

  head = ['Solution', 'Total'] + report.map { |test, scores|
     [test, 'Score', 'Rank']
  }.flatten

  ret = []
  ret << tr(head)
  ret << tr(['---'] * head.length)

  scores_per_solution.each do |solution, ss|
    slink = "[#{solution}](../#{File.basename(solutions[solution])})"
    ret << tr([slink, ss.total] + report.map { |test, _|
      score = ss.scores[test]
      [score.desc, score.score, "##{score.rank}"]
    }.flatten)
  end

  ret.join("\n")
end

summary = JSON.parse(File.read(File.join(SummaryDir, 'summary.json')))

File.open(File.join(SummaryDir, "shortest.md"), 'w') do |f|
  f.puts '# Shortest'
  f.puts
  f.puts gen_table(summary, score_shortest(summary))
end
