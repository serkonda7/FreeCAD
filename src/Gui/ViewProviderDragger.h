/***************************************************************************
 *   Copyright (c) 2017 Werner Mayer <wmayer[at]users.sourceforge.net>     *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#ifndef GUI_VIEWPROVIDER_DRAGGER_H
#define GUI_VIEWPROVIDER_DRAGGER_H

#include "ViewProviderDocumentObject.h"
#include "SoFCCSysDragger.h"
#include <Base/Placement.h>
#include <App/PropertyGeo.h>

class SoDragger;
class SoTransform;

namespace Gui {

namespace TaskView {
    class TaskDialog;
}

class View3DInventorViewer;

/**
 * The base class for all view providers modifying the placement
 * of a geometric feature.
 * @author Werner Mayer
 */
class GuiExport ViewProviderDragger : public ViewProviderDocumentObject
{
    PROPERTY_HEADER_WITH_OVERRIDE(Gui::ViewProviderDragger);

public:
    /// constructor.
    ViewProviderDragger();

    /// destructor.
    ~ViewProviderDragger() override;

    App::PropertyPlacement TransformOrigin;

    Base::Placement getTransformOrigin() const { return TransformOrigin.getValue(); }
    void setTransformOrigin(const Base::Placement& placement);
    void resetTransformOrigin();

public:
    /** @name Edit methods */
    //@{
    bool doubleClicked() override;
    void setupContextMenu(QMenu*, QObject*, const char*) override;
    void updateData(const App::Property*) override;

    ViewProvider *startEditing(int ModNum=0) override;

    /*! synchronize From FC placement to Coin placement*/
    static void updateTransform(const Base::Placement &from, SoTransform *to);

    void updatePlacementFromDragger();
    void updateTransformFromDragger();

    Base::Placement getObjectPlacement() const;
    Base::Placement getDraggerPlacement() const;
    void setDraggerPlacement(const Base::Placement& placement);

protected:
    bool setEdit(int ModNum) override;
    void unsetEdit(int ModNum) override;
    void setEditViewer(View3DInventorViewer*, int ModNum) override;
    void unsetEditViewer(View3DInventorViewer*) override;
    //@}

    void onChanged(const App::Property* prop) override;

    bool forwardToLink();

    /**
     * Returns a newly create dialog for the part to be placed in the task view
     * Must be reimplemented in subclasses.
     */
    virtual TaskView::TaskDialog* getTransformDialog();

    CoinPtr<SoFCCSysDragger> csysDragger = nullptr;
    ViewProvider *forwardedViewProvider = nullptr;

private:
    static void dragStartCallback(void *data, SoDragger *d);
    static void dragFinishCallback(void *data, SoDragger *d);
    static void dragMotionCallback(void *data, SoDragger *d);

    void updateDraggerPosition();

    Base::Placement draggerPlacement { };
};

} // namespace Gui


#endif // GUI_VIEWPROVIDER_DRAGGER_H

