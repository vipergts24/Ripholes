/* Ripholes in Rubbish
 * Save class header
 */

#include <utility>
#include <vector>
#include <PA9.h>

using namespace std;

class Save
{
   public:
       Save ();
      ~Save ();

      // Data functions
      void loadGame ();
      void saveGame ( vector<u8> &vuLvls, vector< pair<u8,bool> > &vpColls );

      // Accessors
      u8     uGetLvls  ();
      bool*  bGetColls ();

   private:
      u8   uLvls;       // Levels available
      bool bColls[6];   // Collectibles
};
