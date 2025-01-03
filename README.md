# Embedded-Soccer-Game
This repository showcases a fully functional embedded soccer game developed using a microcontroller and custom-designed PCB. The game incorporates an interactive gameplay experience with a joystick, slide potentiometer, and four external switches, enabling players to control movement, speed, and actions such as shooting and passing.

In our game, we will have two teams moving across a soccer field--each team will have three field players and one goalkeeper. There will be a switch for the user to choose between controlling the various field players. The user moves the field player across the field using a joystick (the goalkeeper only moves left or right), and uses switches to pass and/or kick the ball. If the ball goes out of bounds past the goalkeeper, the nearest goalkeeper goal kicks. If a goal is made, the ball resets to the center. The first team to score three goals wins. A kicking sound will play when the ball is kicked, and a scoring sound will play when a goal is scored. Additionally, we will have six leds, three green and three red to represent each team. The respective leds will light up when a team scores to keep track of the game score (the score will also be visible on the screen). The user controls team 1 while team 2 moves based on randomized logic, adding unpredictability.

# Features
- Four external buttons: shoot, pass, switch between field players, reset game
- One slide-pot: moves goalkeeper left/right
- One joystick: moves players on field
- Three Sprites: field players, goalkeepers, soccer ball
- Three Sounds: kicking sound, scoring sound, winning sound
- Score: displayed on screen and using LEDs for both teams
- ISRs: Timer G12 for joystick, Systick for sound output
- Language Options: English, Spanish (displayed at beginning of game)

# Hardware Components
- Microcontroller
- Custom PCB
- Joystick
- Slide potentiometer
- Four external switches
- Six LEDs
- Speaker

# Gameplay Mechanics
1. Use the joystick for field player movement.
2. Use the slide potetiometer for goalkeeper movement.
3. Execute actions such as shooting, passing, and switching between field players with the external switches.

## How to Play
- Move your field players using the joystick.
- Switch between field players using switch 4.
- Move your goalkeeper using the slide potentiometer.
- Use switches 1 and 2 to perform shooting and passing actions.
- Score 3 goals to win the game.

# Development Highlights
- Achieved 40% realism in gameplay mechanics, focusing on responsive player movement, speed control, and actions like
shooting and passing.
- Optimized ISRs for real-time game state updates, reducing input lag by 10 milliseconds and enhancing user responsiveness.

# Technologies/Skills Used
- Embedded C & Assembly
- Microcontroller programming
- Printed Circuit Board Design
- Soldering
- Hardware integration and optimization
- OpenGameArt & GNU Octave for sounds

## Contributing
We welcome contributions! If you would like to suggest improvements or report an issue, please create a pull request or submit an issue in the repository.

---
Enjoy your experience with the Embedded Soccer Game! Feel free to reach out with feedback or ideas for enhancements.
