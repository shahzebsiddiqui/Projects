""
Author: Shahzeb Siddiqui
Date: 05/03/2014
Description: This is an implementation of BlackJack card game with only two players. Player versus Dealer. This game does
             not contain all the casino rules for BlackJack such as double down and splitting hands, however the basic 
             functionality of BlackJack is present. In this game Dealer wins on ties and player score depends on the game outcome.
             
Note: This code works with codeskulptor.org and will not work with tradition Python compiler because it uses library simplegui
      which is a custom made library
"""      

import simplegui
import random

# load card sprite - 949x392 - source: jfitz.com
CARD_SIZE = (73, 98)
CARD_CENTER = (36.5, 49)
card_images = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/cards.jfitz.png")

CARD_BACK_SIZE = (71, 96)
CARD_BACK_CENTER = (35.5, 48)
card_back = simplegui.load_image("http://commondatastorage.googleapis.com/codeskulptor-assets/card_back.png")    

# initialize some useful global variables
in_play = False
outcome_action = ""
game_outcome = ""
score = 0
dealer_turn = False
gameover = False
numdeals = 0
# define globals for cards
SUITS = ('C', 'S', 'H', 'D')
RANKS = ('A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K')
VALUES = {'A':1, '2':2, '3':3, '4':4, '5':5, '6':6, '7':7, '8':8, '9':9, 'T':10, 'J':10, 'Q':10, 'K':10}


# define card class
class Card:
    def __init__(self, suit, rank):
        if (suit in SUITS) and (rank in RANKS):
            self.suit = suit
            self.rank = rank
        else:
            self.suit = None
            self.rank = None
            print "Invalid card: ", suit, rank

    def __str__(self):
        return self.suit + self.rank

    def get_suit(self):
        return self.suit

    def get_rank(self):
        return self.rank

    def draw(self, canvas, pos):
        card_loc = (CARD_CENTER[0] + CARD_SIZE[0] * RANKS.index(self.rank), 
                    CARD_CENTER[1] + CARD_SIZE[1] * SUITS.index(self.suit))
        canvas.draw_image(card_images, card_loc, CARD_SIZE, [pos[0] + CARD_CENTER[0], pos[1] + CARD_CENTER[1]], CARD_SIZE)
        
# define hand class
class Hand:        
    
    def __init__(self):                
        self.handlist = []
    def __str__(self):
         self.handlist_str = ""    
         for tok in self.handlist:
                self.handlist_str += " " + str(tok)
         return "Hand contains" + self.handlist_str	# return a string representation of a hand

    def add_card(self, card):
       self.handlist.append(card) 

    def get_value(self):
        self.score = 0
        self.ace_found = False
               
        # loop through all cards in hand
        for card in self.handlist:
            # if Ace in hand
            if card.get_rank() == 'A':
                self.ace_found = True                
            # increment score by lookup value in dictionary VALUES                                    
            self.score += VALUES[card.get_rank()]
        if self.ace_found == True:                        
           if self.score + 10 <= 21: 
               return self.score + 10
           else:
               return self.score
                
                
        return self.score
   
    def draw(self, canvas, pos):
        
        for cards_hand in self.handlist:
            cards_hand.draw(canvas,[pos[0],pos[1]])            
            pos[0]+= 100
            # draw a hand on the canvas, use the draw method for cards
             
# define deck class 
class Deck:        
    def __init__(self):
        self.decklist = []
        for suit in SUITS:
            for rank in RANKS:
                self.card_obj = Card(suit,rank)                
                self.decklist.append(self.card_obj)

    def shuffle(self):
        random.shuffle(self.decklist)

    def deal_card(self):
        return self.decklist.pop(-1)        
    
    def __str__(self):
        self.decklist_str = ""        
        for i in self.decklist:
            self.decklist_str += str(i)
        return "Deck contains" + self.decklist_str



#deal sets up BlackJack with two cards for dealer and player where the hole card is hidden for dealer.
def deal():
    global game_outcome,outcome_action, in_play, player_hand, dealer_hand, deck_obj, dealer_turn, gameover,score, numdeals 
    
    numdeals += 1
    # deal button hit in middle of game, then declare dealer as winner
    if gameover == False and numdeals != 1:
        game_outcome = "Dealer wins, Player forfeit."
        score -= 1
    
    gameover = False    
    dealer_turn = False
    deck_obj = Deck()    
    deck_obj.shuffle()
    
    player_hand = Hand()
    dealer_hand = Hand()
    
    dealer_hand.add_card(deck_obj.deal_card())
    player_hand.add_card(deck_obj.deal_card())
    dealer_hand.add_card(deck_obj.deal_card())
    player_hand.add_card(deck_obj.deal_card())
    
    
    outcome_action = "Hit or Stand?"
    
    
    in_play = True

# Player receives a new card to its hand and the hand score is recomputed
def hit():    
    global score, player_hand, deck_obj, game_outcome, outcome_action, in_play
    game_outcome = ""
    # disable hit button after stand is pressed
    if dealer_turn == False:
        # if the hand is in play, hit the player
        if in_play == True:
            player_hand.add_card(deck_obj.deal_card())
        
        if player_hand.get_value() > 21:
            in_play = False        
            game_outcome =  "Player Busted!!!, Dealer wins"
            outcome_action = "New Deal?"
            score -= 3
        
    

# Once player stands, its dealer turn and outcome of game is determined        
def stand():
    
    global dealer_hand, deck_obj, outcome_action, game_outcome, dealer_turn, score, gameover
    
    dealer_turn = True    
    outcome_action = "New Deal?"
    
    # if player is busted
    if in_play == False:
        game_outcome =  "Player Busted!!!, Dealer wins"
    else:
        # if hand is in play, repeatedly hit dealer until his hand has value 17 or more
        while dealer_hand.get_value() < 17:
            dealer_hand.add_card(deck_obj.deal_card())
    
        #dealer busted, player not busted
        if dealer_hand.get_value() > 21:
            game_outcome = "Dealer busted!!!, Player wins"
            # player gets +3 on dealer bust if player doesnt have 21, otherwise +5
            if player_hand.get_value() != 21:
                score += 3
            else:
                score += 5
        elif dealer_hand.get_value() >= player_hand.get_value():
            game_outcome = "Dealer Wins"
            score -= 1
        else:
            game_outcome = "Player Wins"
            if player_hand.get_value() == 21:
                score += 5
            else:
                score += 1
    
    gameover = True            
    

# draw handler    
def draw(canvas):
    # test to make sure that card.draw works, replace with your code below
            
    player_hand.draw(canvas,[100,450])
    dealer_hand.draw(canvas,[100,200])
    
    if dealer_turn == False:
        canvas.draw_image(card_back, (CARD_BACK_CENTER[0],CARD_BACK_CENTER[1]), (CARD_BACK_SIZE[0],CARD_BACK_SIZE[1]), (100 + CARD_BACK_CENTER[0], 200 + CARD_BACK_CENTER[1]), (CARD_BACK_SIZE[0],CARD_BACK_SIZE[1]))
    if in_play == False:        
        dealer_hand.draw(canvas,[100,200])
    
    canvas.draw_text("Black Jack",[100,50],40,'Red', 'serif')
    canvas.draw_text("Dealer", [100,150], 28, 'Yellow', 'sans-serif')
    canvas.draw_text("Player", [100,400], 28, 'Yellow', 'sans-serif')
    canvas.draw_text(game_outcome,[250,150],25,'Blue')
    canvas.draw_text(outcome_action,[225,400],30,'Blue')
    canvas.draw_text("Score: " + str(score),[400,50],28,'Black')
    
# initialization frame
frame = simplegui.create_frame("Blackjack", 600, 600)
frame.set_canvas_background("Green")

#create buttons and canvas callback
frame.add_button("Deal", deal, 200)
frame.add_button("Hit",  hit, 200)
frame.add_button("Stand", stand, 200)
frame.set_draw_handler(draw)

frame.add_label("Score System")
frame.add_label("Dealer Wins: -1")
frame.add_label("Player Wins: +1")
frame.add_label("Player Bust: -3")
frame.add_label("Dealer Bust: +3")
frame.add_label("Player BlackJack(21): +5")


# start frame and deal hand
deal()
frame.start()
