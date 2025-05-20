typedef struct
{
   int temp;
   void init();
   void loadDefault();
   void newStartMenu();
   void newWorldMap();
   void update();
   void pasteBegin();
   void freezeGame();
   void draw();
   void startPasteSeq();
   void createLevel();
   void newOptionsMenu();
   void setFinLevel();
   bool pageRip();
   void playLevel();
}Game;
