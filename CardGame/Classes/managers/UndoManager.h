#pragma once
#include "models/UndoModel.h"
class UndoManager {
public:
    UndoManager(UndoModel& undoModel) { _undoModel = undoModel; }
    void recordUndoState(const UndoCardState& state) {_undoModel.record(state);}
    bool undo(UndoCardState& outState) {return _undoModel.undo(outState);}
    bool canUndo() const {return _undoModel.canUndo();}
    void clearUndoHistory() {_undoModel.clearHistory();}
    int getUndoSize() {return _undoModel.getSize();}
private:
    UndoModel _undoModel;
};