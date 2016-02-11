# KAUST CS242, Programming Languages, A4, A4Solution.rb
# Name: Shahzeb Siddiqui
# Date: 04/25/2013

class MyTetris < Tetris
  # your enhancements here
	
	#initialize 
	def initialize
		super
		set_board
		key_bindings
		run_game
	end	
	
	#set_board instantiates MyBoard class and creates the Tetris Canvas for game
    def set_board
		@canvas = TetrisCanvas.new
		@board = MyBoard.new(self)
		@canvas.place(@board.block_size * @board.num_rows + 3,
					  @board.block_size * @board.num_columns + 6, 12, 80)
		@board.draw
    end
	
	#key_bindings attach keys with game operation. Added keys '/','c'
	def key_bindings
		@root.bind('n', proc {self.new_game}) 
		@root.bind('p', proc {self.pause}) 
		@root.bind('q', proc {exitProgram})    
		@root.bind('a', proc {@board.move_left})
		@root.bind('Left', proc {@board.move_left})     
		@root.bind('d', proc {@board.move_right})
		@root.bind('Right', proc {@board.move_right}) 
		@root.bind('s', proc {@board.rotate_clockwise})
		@root.bind('Down', proc {@board.rotate_clockwise})
		@root.bind('w', proc {@board.rotate_counter_clockwise})
		@root.bind('Up', proc {@board.rotate_counter_clockwise}) 	
		@root.bind('space' , proc {@board.drop_all_the_way}) 	
		@root.bind('/', proc {@board.rotate_180})     	
		@root.bind('c', proc {@board.bonus_piece})     

    end
    
	#buttons draws Tetris buttons from existing feature. Added Bonus Timer and Multiplier Label for new feature	
	def buttons
		pause = TetrisButton.new('pause', 'lightcoral'){self.pause}
		pause.place(35, 50, 90, 7)

		new_game = TetrisButton.new('new game', 'lightcoral'){self.new_game}
		new_game.place(35, 75, 15, 7)
		
		quit = TetrisButton.new('quit', 'lightcoral'){exitProgram}
		quit.place(35, 50, 140, 7)
		
		move_left = TetrisButton.new('left', 'lightgreen'){@board.move_left}
		move_left.place(35, 50, 27, 536)
		
		move_right = TetrisButton.new('right', 'lightgreen'){@board.move_right}
		move_right.place(35, 50, 127, 536)
		
		rotate_clock = TetrisButton.new('^_)', 'lightgreen'){@board.rotate_clockwise}
		rotate_clock.place(35, 50, 77, 501)

		rotate_counter = TetrisButton.new('(_^', 'lightgreen'){
		  @board.rotate_counter_clockwise}
		rotate_counter.place(35, 50, 77, 571)
		
		drop = TetrisButton.new('drop', 'lightgreen'){@board.drop_all_the_way}
		drop.place(35, 50, 77, 536)

		label = TetrisLabel.new(@root) do
		  text 'Current Score: '   
		  background 'lightblue'
		end
		label.place(30, 100, 26, 45)
		@score = TetrisLabel.new(@root) do
		  background 'lightblue'
		end
		@score.text(@board.score)
		@score.place(35, 50, 126, 45)
		
		# ADDED FEATURES: BONUS TIMER
		bonuslabel = TetrisLabel.new(@root) do
		  text 'Bonus Timer: '   
		  background 'lightblue'
		end
		
		bonuslabel.place(35, 100, 200, 45)
		@bonustimerLabel = TetrisLabel.new(@root) do
		  background 'lightblue'
		end
		
		def updatebonustime
			@bonustimerLabel.text(@board.getbonustimerleft)
			@bonustimerLabel.place(35,50,300,45)
		end
		
		# ADDED FEATURES: SCORE MULTIPLIER
		scoremultipilerlabel = TetrisLabel.new(@root) do
			text 'Score Multiplier: '
			background 'lightblue'
		end
		
		scoremultipilerlabel.place(35,100,200,80)
		@scoremultipliervalue = TetrisLabel.new(@root) do
			background 'lightblue'
		end
		
		def showscoremultiplier
			@scoremultipliervalue.text(@board.getscoremultiplier)
			@scoremultipliervalue.place(35,50,300,80)
		end	
		
	end
  #run_game modified to show score multiplier and update bonus timer each timer iteration
  def run_game
    if !@board.game_over? and @running      	  
	 showscoremultiplier 
	  if @board.getmultiplier
		@board.bonustimer
		updatebonustime		
	  end	
	  @timer.stop
      @timer.start(@board.delay, (proc{@board.run; run_game}))
    end
  end
 
end

class MyPiece < Piece
    #initialize acquire methods from inherited class using super
	def initialize (point_array, board)
		super
		@all_rotations = point_array
		@board = board		
	end
  # class method to choose the next piece
  def self.next_piece (board)
    MyPiece.new(All_Pieces.sample, board)	
  end
  
  # method for choosing the special piece using cheat option 
  def self.special_piece (board)
	MyPiece.new(BonusPiece.sample,board);
  end
  
  def current_rotation
    @all_rotations[@rotation_index]
  end
  
  # class array holding all the pieces and their rotations
  All_Pieces = [[[[0, 0], [1, 0], [0, 1], [1, 1]]],  # square (only needs one)
               rotations([[0, 0], [-1, 0], [1, 0], [0, -1]]), # T               
			   [[[0, 0], [-1, 0], [1, 0], [2, 0]], # long (only needs two)
               [[0, 0], [0, -1], [0, 1], [0, 2]]],			   
               rotations([[0, 0], [0, -1], [0, 1], [1, 1]]), # L
               rotations([[0, 0], [0, -1], [0, 1], [-1, 1]]), # inverted L
               rotations([[0, 0], [-1, 0], [0, -1], [1, -1]]), # S
               rotations([[0, 0], [1, 0], [0, -1], [-1, -1]]),    # Z
			   rotations([[0, 0], [-1, 0], [1, 0], [0, -1], [0, 1]]), # piece 1
			   rotations([[0, 0], [0, 1], [1, 1,], [0, 0]]),  #piece 2
			   rotations([[0, 0], [0, 1], [1, 1], [2, 1], [2, 0]]),  #piece 3
			   rotations([[0,0],[0,1],[0,2],[1,0],[2,0]]),  #L
			   rotations([[0,0],[0,1],[1,1],[2,1],[2,0],[3,1],[4,0],[4,1]])] #E
	
	# special piece when using cheat option
	BonusPiece = [rotations([[0,0],[0,0],[0,0],[0,0]])]
  
end

class MyBoard < Board
  # your enhancements here
  #initiaize acquire methods from Board using super. Added bonustimerleft, scoremultiplier. Default value for scoremultiplier = 1
  def initialize (game)
	super
	current_block = MyPiece.next_piece(self)
	@bonustimerleft = 90	
	@multiplier = false
	@scoremultiplier = 1
  end
  
  #get bonustimerleft 
  def getbonustimerleft
	@bonustimerleft
  end
  #get multiplier   
  def getmultiplier
	@multiplier
  end
  #get scoremultiplier
  def getscoremultiplier
	@scoremultiplier
  end
  #set multiplier to false after timer runs out
  def setmultiplierfalse
	@multiplier = false
  end
  
  #rotate piece by 180 
  def rotate_180
    if !game_over? and @game.is_running?
      @current_block.move(0, 0, -2)
    end
    draw
  end
  # user using cheat option using 'c' key. Checks if score is greater than 100 and enables cheat feature
  def bonus_piece
	if @score >= 100
		@score = @score - 100
		@cheat = true
	end
  end
  #run drops piece by one each timer. Modified code to check if special piece is next if cheat is enabled
  def run    
    ran = @current_block.drop_by_one	
    if !ran
      store_current	  
      if !game_over?        
		if @cheat
		  special_piece
		else
			next_piece
		end
      end
    end
    @game.update_score
    draw
  end
  
  def drop_all_the_way
    if @game.is_running?
      ran = @current_block.drop_by_one
      while ran
        @current_pos.each{|block| block.remove}
        #modified score value by scoremultiplier 
		@score = @score + 1*@scoremultiplier
        ran = @current_block.drop_by_one
      end
      draw
      store_current
      if !game_over?
	    if @cheat
			special_piece
		else
			next_piece
		end
      end
      @game.update_score
      draw
    end
  end
  
  # removes all filled rows and replaces them with empty ones, dropping all rows
  # above them down each time a row is removed and increasing the score.  
  def remove_filled
    (2..(@grid.size-1)).each{|num| row = @grid.slice(num);
      # see if this row is full (has no nil)
      if @grid[num].all?
	    #Start bonus timer and score multiplier when row is removed
		@multiplier = true
	    @scoremultiplier = 2
		@bonustimerleft = 90
        # remove from canvas blocks in full row
        (0..(num_columns-1)).each{|index|
          @grid[num][index].remove;
          @grid[num][index] = nil
        }
        # move down all rows above and move their blocks on the canvas
        ((@grid.size - num + 1)..(@grid.size)).each{|num2|
          @grid[@grid.size - num2].each{|rect| rect && rect.move(0, block_size)};
          @grid[@grid.size-num2+1] = Array.new(@grid[@grid.size - num2])
        }
        # insert new blank row at top
        @grid[0] = Array.new(num_columns);
        # adjust score by scoremultiplier
        @score = @score + 10*@scoremultiplier;
      end}	  
    self
	
  end
  #special_piece assigns special piece to currentblock for next piece and sets cheat to false
  def special_piece
	@current_block = MyPiece.special_piece(self)
	@current_pos = nil
	@cheat = false
  end
  #next_piece gets new piece and is stored in current_block
  def next_piece
	@current_block = MyPiece.next_piece(self)  
	@current_pos = nil
  end
  #bonustimer decrements bonustimer by 1 until it reaches 0 and sets scoremultiplier = 1
  def bonustimer    
	@bonustimerleft -= 1
	if @bonustimerleft == 0
		@multiplier = false
		@scoremultiplier = 1
	end	
  end

end
