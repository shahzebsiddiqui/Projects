# KAUST CS242, Programming Languages, A4, A4Runner.rb

require_relative './A4Provided'
require_relative './A4Solution'

def runTetris
  Tetris.new 
  mainLoop
end

def runMyTetris
  MyTetris.new
  mainLoop
end

if ARGV.count == 0
  runMyTetris
elsif ARGV.count != 1
  puts "usage: A4Runner.rb [enhanced | original]"
elsif ARGV[0] == "enhanced"
  runMyTetris
elsif ARGV[0] == "original"
  runTetris
else
  puts "usage: A4Rrunner.rb [enhanced | original]"
end

