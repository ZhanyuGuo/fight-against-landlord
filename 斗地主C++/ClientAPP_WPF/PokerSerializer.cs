using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ClientAPP_WPF
{
    public class Poker
    {
        public enum PokerColor { Spade, Heart, Diamond, Club, Joker }
        public int ID { get; set; }
        public int Point { get 
            { 
                if (this.ID == 53) { return 14; }
                if (this.ID == 54) { return 15; }
                return (this.ID - 1) % 13 + 1;
            } }
        public PokerColor Color { get
            {
                if (this.ID == 53 || this.ID == 54) { return PokerColor.Joker; }
                return (PokerColor)((this.ID - 1) / 13);
            } }
        public override string ToString()
        {
            if (this.ID == 53)
            {
                return "Joker";
            }
            if (this.ID == 54)
            {
                return "JOKER";
            }
            else
            {
                string colorChar = this.GetColorChar();
                return colorChar + this.GetPointChar();    
            }
            
        }
        public Poker(int ID)
        {
            this.ID = ID;
        }
        private string GetColorChar()
        {
            switch (this.Color)
            {
                case PokerColor.Spade:
                    return "♠";
                case PokerColor.Heart:
                    return "♥";
                case PokerColor.Club:
                    return "♣";
                case PokerColor.Diamond:
                    return "♦";
                default:
                    return "";
            }

        }
        private string GetPointChar()
        {
            switch (this.Point)
            {
                case 1:
                    return "A";
                case 11:
                    return "J";
                case 12:
                    return "Q";
                case 13:
                    return "K";
                default:
                    return this.Point.ToString();
            }

        }
    }

    class PokerSerializer
    {
        public static sbyte[] Serialize(IEnumerable<Poker> pokers)
        {
            int count = pokers.Count();
            sbyte[] buf = new sbyte[count];
            int index = 0;
            foreach(Poker poker in pokers)
            {
                buf[index] = (sbyte)poker.ID;
                index++;
            }
            return buf;
        }
        public static List<Poker> Deserialize(sbyte[] pokerIDs)
        {
            List<Poker> pokers = new List<Poker>();
            foreach (sbyte id in pokerIDs)
            {
                if (id == 0)
                {
                    break;
                }
                pokers.Add(new Poker(id));
            }
            return pokers;
        }
        public static List<Poker> Deserialize(byte[] pokerIDs)
        {
            List<Poker> pokers = new List<Poker>();
            foreach (byte id in pokerIDs)
            {
                if (id == 0)
                {
                    break;
                }
                pokers.Add(new Poker(id));
            }
            return pokers;
        }
        public static string DecodePokersToString(IEnumerable<Poker> pokers)
        {
            return PokerSerializer.DecodePokersToString(pokers, 3);
        }
        public static string DecodePokersToString(IEnumerable<Poker> pokers, int colCapicity)
        {
            StringBuilder sb = new StringBuilder();
            int i = 0;
            foreach (var poker in pokers)
            {
                sb.Append(poker.ToString());
                i++;
                if (i % 3 == 0) 
                {
                    sb.Append("\n");
                }
            }
            return sb.ToString();
        }
    }
}
