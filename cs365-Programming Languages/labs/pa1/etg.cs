using System;
using System.IO;
using System.Collections.Generic;

struct Coords {
    public int x;
    public int y;

    public Coords(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public override bool Equals(Object obj) {
        return obj is Coords c && this == c;
    }

    public override int GetHashCode() {
        return this.x.GetHashCode() ^ this.y.GetHashCode();
    }

    public static bool operator ==(Coords a, Coords b) {
        return a.x == b.x && a.y == b.y;
    }

    public static bool operator !=(Coords a, Coords b) {
        return !(a == b);
    }
}

public class Level
{
    private Location[,] _locations;

    public int x;
    public int y;

    public Level(int numRows, int numColumns)
    {
        this.x = numRows;
        this.y = numColumns;
        _locations = new Location[numRows, numColumns];
        for (int row = 0; row < numRows; row++)
        {
            for (int column = 0; column < numColumns; column++)
            {
                _locations[row, column] = new Location();
            }
        }
    }

    public Location GetLocation(int row, int column)
    {
        return _locations[row, column];
    }

    public void AddExitLocation(int x, int y)
    {
        _locations[x, y] = new ExitLocation();
    }
}

public class Location
{
    public bool isexitloc;
    public bool has_skelly;
    public List<Entity> _entities;

    public Location()
    {
        _entities = new List<Entity>();
        isexitloc = false;
    }

    public void AddEntity(Entity entity)
    {
        _entities.Add(entity);
    }

    public void RemoveEntity(Entity entity)
    {
        _entities.Remove(entity);
    }

    public virtual void Inspect()
    {
        if(_entities.Count > 0){
            if(this.has_skelly && _entities.Count > 1){
                
                foreach (Entity entity in _entities)
                {
                    entity.Look();
                }
            }else if(this.has_skelly == false && _entities.Count > 0){

                foreach (Entity entity in _entities)
                {
                    entity.Look();
                }
            }else{

                Console.WriteLine("Not much to see here.");
            }

        }else{

            Console.WriteLine("Not much to see here.");
        }
    }

    public virtual void Occupy()
    {
        foreach (Entity entity in _entities)
        {
            entity.Interact();
        }
    }
}

public class ExitLocation : Location
{

    public ExitLocation()
    {
        isexitloc = true;
    }

    public override void Inspect()
    {
        Console.WriteLine("That looks like the gate out of this spooky place!");
    }

    public override void Occupy()
    {
        if(Player.has_key){

            Console.WriteLine("You open the gate with your key!");
        }else{

             Console.WriteLine("You try to open the gate, but it's locked. Must need a key...");
        }
    }
}

public abstract class Entity
{
    public abstract void Look();

    public abstract void Interact();
}

public class Key : Entity
{
    public bool grabbed = false;
    public override void Look()
    {
        if(grabbed == false){
            Console.WriteLine("You see a key on the ground! Might need that to get out of here...");
        }
    }

    public override void Interact()
    {
        if(grabbed == false){

            Console.WriteLine("You picked up a key!");
            Player.has_key = true;
            grabbed = true;
        }
    }
}

public class Loot : Entity
{
    private int _coinCount;
    public bool grabbed = false;

    public Loot(int coinCount)
    {
        _coinCount = coinCount;
    }

    public override void Look()
    {
        if(grabbed == false){

            Console.WriteLine("You see what looks like the corner of a treasure chest poking out of the ground.");
        }else{

            Console.WriteLine("A treasure chest sits already opened.");
        }
    }

    public override void Interact()
    {
        if(grabbed == false){

            Console.WriteLine($"You open the chest and find {_coinCount} coins!");
            Player.add_player_loot(_coinCount);
            grabbed = true;
        }else{

            Console.WriteLine("The chest is empty...");
        }
    }
}

public class Skeleton : Entity
{
    public override void Look()
    {
        
    }

    public override void Interact()
    {
        Console.WriteLine("A bony arm juts out of the ground and grabs your ankle!");
        Console.WriteLine("You've been dragged six feet under by a skeleton.");
    }
}

class Player {
    public Coords coords { get; set; }
    public static bool has_key {get; set;}
    public static int player_loot = 0;
    public bool is_dead = false;

    public bool is_alive() {
        
        if(this.is_dead){

            return false;
        }
        return true;
    }

    public Player() {
        this.coords = new Coords(0, 0);
    }

    public bool is_at(Coords xy) {
        return this.coords == xy;
    }

    public static void add_player_loot(int coins){

        player_loot += coins;
    }
    public void print_stats() {
        Console.WriteLine($"  LOCATION: {this.coords.x}, {this.coords.y}");
        Console.WriteLine($"  COINS:    {player_loot}");
        Console.WriteLine($"  KEY:      {has_key}");
        Console.WriteLine($"  DEAD:     {this.is_dead}");
    }
}

class Game {
    int    num_turns;
    int    exhaustion;
    Level  level;
    Key    key;
    Loot   loot;
    Skeleton skeleton;
    public Player player { get; }

    public Game() {
        this.player = new Player();
    }

    public void load(string path) {

        string line;
        using (StreamReader reader = new StreamReader(path)) {
            while ((line = reader.ReadLine()) != null) {
                if (line == "") { continue; }

                string[] split = line.Split(' ', StringSplitOptions.RemoveEmptyEntries);

                if (split.Length < 3) {
                    Console.WriteLine($"Bad command in level file: '{line}'");
                    Environment.Exit(1);
                }

                int x     = int.Parse(split[1]);
                int y     = int.Parse(split[2]);
                int count = 0;

                if (split.Length > 3) {
                    count = int.Parse(split[3]);
                }

                switch (split[0]) {
                    case "size":
                        this.level = new Level(x, y);
                        break;
                    case "exit":
                        level.AddExitLocation(x, y);
                        break;
                    case "key":
                        this.key = new Key();
                        level.GetLocation(x, y).AddEntity(key);
                        break;
                    case "loot":
                        this.loot = new Loot(count);
                        level.GetLocation(x, y).AddEntity(loot);
                        break;
                    case "skeleton":
                        this.skeleton = new Skeleton();
                        level.GetLocation(x, y).AddEntity(skeleton);
                        level.GetLocation(x, y).has_skelly = true;
                        break;
                    default:
                        Console.WriteLine($"Bad command in level file: '{line}'");
                        Environment.Exit(1);
                        break;

                }

                this.exhaustion = (2 * level.x * level.y);
            }
        }
    }

    public void input(string line) {
        this.num_turns += 1;

        // Check for exhaustion
        Console.WriteLine("================================================================");

        string[] split = line.Split(' ', StringSplitOptions.RemoveEmptyEntries);

        if (split.Length != 2) {
            Console.WriteLine($"Bad command in input: '{line}'");
            return;
        }

        Coords new_coords = this.player.coords;
        switch (split[1]) {
            case "north":
                new_coords.y += 1;
                break;
            case "south":
                new_coords.y -= 1;
                break;
            case "east":
                new_coords.x += 1;
                break;
            case "west":
                new_coords.x -= 1;
                break;
            default:
                Console.WriteLine($"Bad command in input: '{line}'");
                return;
        }

        // Are the new coords valid?
        switch (split[0]) {
            case "go":
                if((new_coords.x >= level.x) || (new_coords.y >= level.y) || (new_coords.x < 0) || (new_coords.y < 0)){

                    Console.WriteLine("A towering wall is before you. This must be the edge of the graveyard.");
                }else{

                    this.player.coords = new_coords;
                    level.GetLocation(this.player.coords.x, this.player.coords.y).Inspect();
                    level.GetLocation(this.player.coords.x, this.player.coords.y).Occupy();
                }
                // Need to look at the new location and interact with it.
                break;
            case "look":
                if((new_coords.x >= level.x) || (new_coords.y >= level.y) || (new_coords.x < 0) || (new_coords.y < 0)){

                    Console.WriteLine("A towering wall is before you. This must be the edge of the graveyard.");
                }else{
                    level.GetLocation(new_coords.x, new_coords.y).Inspect();
                }
                break;
            default:
                Console.WriteLine($"Bad command in input: '{line}'");
                return;
        }

        this.exhaustion -= 1;
        if(this.exhaustion == 0){

            this.num_turns += 1;
            Console.WriteLine($"{this.player.coords.x}, {this.player.coords.y}> You have died from exhaustion.");
            this.player.is_dead = true;
        }
        if(level.GetLocation(this.player.coords.x, this.player.coords.y).has_skelly){

            this.player.is_dead = true;
        }
    }

    bool is_over() {
        
        if(level.GetLocation(this.player.coords.x, this.player.coords.y).isexitloc && Player.has_key){

            return true;
        }else if(this.player.is_dead){

            return true;
        }else{

            return false;
        }
    }

    void print_stats() {
        if (this.is_over() && player.is_alive()) {
            Console.WriteLine("You successfully escaped the graveyard!");
        } else {
            Console.WriteLine("You did not escape the graveyard. GAME OVER");
        }
        Console.WriteLine($"Game ended after {this.num_turns} turn(s).");
        player.print_stats();
    }

    public void exit() {
        Console.WriteLine("================================================================");
        this.print_stats();
        Environment.Exit(0);
    }

    public void exit_if_over() {
        if (this.is_over()) { this.exit(); }
    }

    public void intro() {
        Console.WriteLine("You awake in a daze to find yourself alone in the dead of night, surrounded by headstones...");
        Console.WriteLine("You must escape this graveyard.");
        Console.WriteLine("================================================================");
        level.GetLocation(this.player.coords.x, this.player.coords.y).Inspect();
        level.GetLocation(this.player.coords.x, this.player.coords.y).Occupy();
        Console.Write($"{this.player.coords.x}, {this.player.coords.y}> ");
    }
}

class ETG {
    static void Main(string[] args) {
        if (args.Length != 1) {
            Console.WriteLine("ERROR: expected a single argument (the level file)");
            Environment.Exit(1);
        }

        Game game = new Game();

        game.load(args[0]);
        game.intro();

        game.exit_if_over();

        string line;

        while ((line = Console.ReadLine()) != null) {
            if (line == "") { continue; }
            game.input(line);
            game.exit_if_over();
            Console.Write($"{game.player.coords.x}, {game.player.coords.y}> ");
        }

        game.exit();
    }
}
