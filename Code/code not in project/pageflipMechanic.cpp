// Put into the Level Class
// Page flip mechanic
void Level::bFlip(bool bFlipPrev)
{
   pair<s32,s32> pAvaPos = avatar->getPos();
   bool bOnHole = false;

   /*---Player wants to flip to the prev page---------------------------------------------------*/

   if (bFlipPrev)			
   {
      // continue Page flip if player is not on first Page
      if ( (*itCurrPage)->uGetPageId() > 1 )		
      {
         // subtract one Page to get new Page
         --itCurrPage;

         // Following code block: move the avatar if possible
         // if the current page is healthy
         if ( (*itCurrPage)->phGetPH()->bIsHealthy() )
         {
            vector<LevelObject*>* vloHoleList = (*itCurrPage)->vloGetHoleList();
            vector<LevelObject*>::iterator itHoles = vloHoleList->begin();

            // iterate through the list of holes to see if the player can move through it
            for (; itHoles != vloHoleList->end(); ++itHoles)
            {
               bOnHole = bCheckPos ( avatar, (*itHoles) ); // returns true if player is on hole

               // if the player is behind a hole, move the avatar
               if ( bOnHole )
               {
                  // change avatar's page to the new page if she can be moved
                  avatar->setCurrPage((*itCurrPage)->uGetPageId());
               }
            }  
         }

         // the page is NOT healthy, therefore the currPage has to be flipped to the page before it.
         else 
         {
            bFlip(true);
         }
      }

      // cannot flip to prev page because avatar is ON the first page
      else
      {
         // code to play error sound, cueing that the player cannot flip in that direction
      }
   }

   /*---Player wants to flip to the next page------------------------------------------------------*/

   else 
   {
      // continue if player is not on the last page
      if ((*itCurrPage)->uGetPageId() != vpPageList->size() ) 
      {
         // add a page to get the new page (the next page)
         ++itCurrPage;

         // Following code block: move the avatar if possible
         // if the current page is healthy
         vector<LevelObject*>* vloHoleList = (*itCurrPage)->vloGetHoleList();
         vector<LevelObject*>::iterator itHoles = vloHoleList->begin();

         if ( (*itCurrPage)->phGetPH()->bIsHealthy() )
         {
            // iterate through the list of holes to see if the player can move through it
            for (; itHoles != vloHoleList->end(); ++itHoles)
            {
               bOnHole = bCheckPos ( avatar, (*itHoles) );     // returns true if player is on hole

               // if the player is behind a hole, move the avatar
               if ( bOnHole )
               {
                  // change avatar's page to the new page if she can be moved
                  avatar->setCurrPage((*itCurrPage)->uGetPageId());
               }
            }  
         }

         // the page is NOT healthy, therefore the player has to be flipped to the page before it.
         else
         {
            bFlip(false);
         }
      }

      // cannot flip to next page because avatar is ON the last page
      else
      {
         // code to play error sound, cueing that the player cannot flip in that direction
      }
   }
}

// returns true if the avatar is able to traverse page
bool bCheckPos ( Player* pAva, LevelObject* loHole)
{
   pair<s32,s32> posAva = pAva->getPos();
   pair<s32,s32> posHole = loHole->getPos();

   pair<s23,s32> posAvaMax;
   posAvaMax.first = posAva.first + posAva->getSprite()->getWidth();
   posAvaMax.second = posAva.second + posAva->getSprite()->getHeight();

   pair<s32,s32> posHoleMax;
   posHoleMax.first = posHole.first + loHole->getSprite()->getWidth();
   posHoleMax.first = posHole.second + loHole->getSprite()->getHeight();

   if ( ( posAva.first > posHole.first ) && ( posAva.first < posHoleMax.first ) )
   {
      if ( ( posAva.second > posHole.second ) && ( posAva.second < posHoleMax.second ) )
      {
         return true;
      }

      if ( ( posAvaMax.second > posHole.second ) && ( posAvaMax.second < posHoleMax.second ) )
      {
         return true;
      }
   }

   if ( ( posAvaMax.first > posHole.first ) && ( posAvaMax.first < posHoleMax.first ) )
   {
      if ( ( posAva.second > posHole.second ) && ( posAva.second < posHoleMax.second ) )
      {
         return true;
      }

      if ( ( posAvaMax.second > posHole.second ) && ( posAvaMax.second < posHoleMax.second ) )
      {
         return true;
      }
   }

   return false;
}
