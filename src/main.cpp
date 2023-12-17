#include <Arduino.h>
#include <math.h>

int D1 = 12;
int D2 = 9;
int D3 = 8;
int D4 = 6;
int digits[4] = {D1, D2, D3, D4};

int A = 11;
int B = 7;
int C = 4;
int D = 2;
int E = 14;
int F = 10;
int G = 5;
int DP = 3;
int segments[8] {A, B, C, D, E, F, G, DP};

int numbers[11][7] = {
  {A, B, C, D, E, F, -1},
  {B, C, -1, -1, -1, -1, -1},
  {A, B, D, E, G, -1, -1},
  {A, B, C, D, G, -1, -1},
  {B, C, F, G, -1, -1, -1},
  {A, C, D, F, G, -1, -1},
  {A, C, D, E, F, G, -1},
  {A, B, C, -1, -1, -1, -1},
  {A, B, C, D, E, F, G},
  {A, B, C, D, F, G, -1},
  {G, -1, -1, -1, -1, -1, -1}
};

class Seven_segments_four_digit {
  private:
    short int counter;
    short int turn;
    int segments[4];
    int digits[4];
    int A, B, C, D, E, F, G, DP;
  public:
    
    int debug() {
      return segments[0];
    }

    Seven_segments_four_digit(
      int D1,int D2, int D3, int D4,
      int A, int B, int C, int D, int E, int F, int G, int DP,
      short int counter = 0
      ) {
        this->counter = counter;
        this->digits[0] = D1, this->digits[1] = D2, this->digits[2] = D3, this->digits[3] = D4;
        this-> A = A, this->B = B, this-> C = C, this->D = D, this->E = E, this->F = F, this->G = G, this->DP = DP;
        this->turn = 0;
    }

    void setup() {
      int outputs[12] = {this->digits[0], this->digits[1], this->digits[2], this->digits[3], this->A, this->B, this->C, this->D, this->E, this->F, this->G, this->DP};
      for (int pin : outputs) {
        pinMode(pin, OUTPUT);
      }
    }

    void increment(int increment = 1) {
      if (this->counter + increment <= 9999) {
        this->counter += increment;
      }
    };

    void decrement(int decrement = 1) {
      if (this->counter - decrement >= 0) {
        this->counter -= decrement;
      }
    };


    void set(int value) {
      if (value >= 9999) {this->counter = 9999;}
      else if (value <= 0) {this->counter = 0;}
      else {this->counter = value;}
    };

    
    void reset() {
      this->set(0);
    };

    void convert_counter_to_segments() {
      int cache = 0;
      int counter = this->counter;
      for (short int i = 0; i < 4; i++) {
        int segment = (counter - (floor(counter/pow(10, i+1))*pow(10, i+1)) - cache)/(pow(10, i));
        cache += segment;
        this->segments[i] = segment;
      }
    };

    void pass_a_turn() {
      if (this->turn < 3) {this->turn++;}
      else {this->turn = 0;}
    }

    void enable_right_segments_for_digit(int digit) {
      int segmentsPINs[8] = {this->A, this->B, this->C, this->D, this->E, this->F, this->G, this->DP};
      int cache = 0;
      for (int pin : segmentsPINs) {
        if (numbers[this->segments[digit]][cache] == pin) {
          digitalWrite(pin, HIGH);
          cache++;
        } else {
        digitalWrite(pin, LOW);
        }
      }
    }

    void enable_right_digit_for_turn() {
      if (this->turn == 0) {digitalWrite(this->digits[0], HIGH); digitalWrite(this->digits[3], LOW);}
      else if (this->turn == 1) {digitalWrite(this->digits[3], HIGH);digitalWrite(this->digits[2], LOW);}
      else if (this->turn == 2) {digitalWrite(this->digits[2], HIGH);digitalWrite(this->digits[1], LOW);}
      else if (this->turn == 3) {digitalWrite(this->digits[1], HIGH);digitalWrite(this->digits[0], LOW);}
    }

    void send_outputs() {
      convert_counter_to_segments();
      enable_right_segments_for_digit(this->turn);
      enable_right_digit_for_turn();
      pass_a_turn();
    };
};

Seven_segments_four_digit ssfg(D1, D2, D3, D4, A, B, C, D, E, F, G, DP, 0);
int test = 0;

void setup() {
  Serial.begin(9600);
  ssfg.setup();
}

void loop() {
  ssfg.set(23); // 1-4 work just fine, 5-x doesn't.
  ssfg.send_outputs();
}

