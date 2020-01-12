// Types
type player =
  | PlayerOne
  | PlayerTwo;

type point =
  | Love
  | Fifteen
  | Thirty;

type pointsData = {
  playerOne: point,
  playerTwo: point
};

type fortyData = {
  player: player, /* The player who have forty points */
  otherPlayerPoint: point
};

type score =
| Points(pointsData)
| Forty(fortyData)
| Deuce
| Advantage(player)
| Game(player);
// Types

// Transitions
let other = player =>
  switch player {
  | PlayerOne => PlayerTwo
  | PlayerTwo => PlayerOne
  };

let incrementPoint: point => option(point) =
  point =>
    switch point {
    | Love => Some(Fifteen)
    | Fifteen => Some(Thirty)
    | Thirty => None
    };

let pointTo = (player, point, current) =>
  switch player {
  | PlayerOne => {...current, playerOne: point}
  | PlayerTwo => {...current, playerTwo: point}
  };

let pointFor = (player, current) =>
  switch player {
  | PlayerOne => current.playerOne
  | PlayerTwo => current.playerTwo
  };
// Transitions

// Scores
let scoreWhenPoints = (current, winner) =>
  switch (current |> pointFor(winner) |> incrementPoint) {
  | Some(np) => Points(pointTo(winner, np, current))
  | None =>
    Forty({
      player: winner,
      otherPlayerPoint: current |> pointFor(other(winner))
    })
  };

let scoreWhenForty = (current, winner) =>
  current.player == winner ?
    Game(winner) :
    (
        switch (incrementPoint(current.otherPlayerPoint)) {
        | Some(p) => Forty({...current, otherPlayerPoint: p})
        | None => Deuce
        }
    );

let scoreWhenDeuce: player => score = winner => Advantage(winner);

let scoreWhenAdvantage: (player, player) => score =
  (advantagedPlayer, winner) =>
    advantagedPlayer == winner ? Game(winner) : Deuce;

let scoreWhenGame = winner => Game(winner);

let score = (current, winner) =>
  switch current {
  | Points(p) => scoreWhenPoints(p, winner)
  | Forty(f) => scoreWhenForty(f, winner)
  | Deuce => scoreWhenDeuce(winner)
  | Advantage(a) => scoreWhenAdvantage(a, winner)
  | Game(g) => scoreWhenGame(g)
  };
 // Scores


let newGame = Points({playerOne: Love, playerTwo: Love});


// Questions
let string_of_player: (player) => string = player =>
    switch player {
    | PlayerOne => "player one"
    | PlayerTwo => "player two"
    };

let string_of_point: (point) => string = point =>
    switch point {
    | Love => "0"
    | Fifteen => "15"
    | Thirty => "30"
    };

let string_of_score: (score) => string = score =>
    switch score {
    | Points(pointsData) => string_of_player(PlayerOne) ++ " " ++ string_of_point(pointsData.playerOne) ++ "|" ++ string_of_point(pointsData.playerTwo) ++ " " ++ string_of_player(PlayerTwo)
    | Forty(fortyData) => string_of_player(fortyData.player) ++ " 40|" ++ string_of_point(fortyData.otherPlayerPoint) ++ " " 
    ++ string_of_player(other(fortyData.player))
    | Deuce => string_of_player(PlayerOne) ++ " 40|40 " ++ string_of_player(PlayerTwo)
    | Advantage(player) => "Advantage " ++ string_of_player(player)
    | Game(player) => string_of_player(player) ++ " wins"
    };
// Questions
