#include "cwhiteboard.h"
#include "cwhiteboardeditorbar.h"
#include "cwhiteboardview.h"

void createWhiteBoardWidgets(CWhiteBoardView **view, CWhiteBoardEditorBar **editorBar)
{
    CWhiteBoardView *whiteBoardView = new CWhiteBoardView;
    CWhiteBoardEditorBar *witeBoardEditorBar = new CWhiteBoardEditorBar(whiteBoardView);
    *view = whiteBoardView;
    *editorBar = witeBoardEditorBar;
}
