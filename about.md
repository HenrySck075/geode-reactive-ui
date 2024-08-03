Casual users should not download this. It does nothing.

This does not mean if you found this mod in your downloaded list then you should remove it. Other mods might depends on this

------


hey uhh 

have you ever wonder "chat how the heck do i automatically put my node to the top right corner without trying to get the parent size every update" 

well here you go

to set position now you can use 
```
auto style = Style::create(node);
# some style change, like this
style->offset = ccp(9,9);
```
profit!!!!

to change applying node just do
```
style->setApplyingNode(node2);
```

which actually does `style->setApplyingNode(node); node->setUserObject("reactive_size", style);` but i know you wont have that much ability to remember to do that tho

consult your ide intellisense for available values
