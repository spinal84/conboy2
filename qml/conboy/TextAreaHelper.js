/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/

function findFlickable(component) {
    var nextParent = component
    while(nextParent) {
        if(nextParent.flicking !== undefined && nextParent.flickableDirection !== undefined)
            return nextParent

        nextParent = nextParent.parent
    }
    return null
}

function animateContentY(animation, flickable, newContentY) {
    animation.target = flickable
    animation.to = newContentY
    animation.running = true
}

function locateFlickableY(flickable) {
    switch(screen.currentOrientation) {
    case Screen.Landscape:
        return flickable.mapToItem(null, flickable.x, flickable.y).y

    case Screen.LandscapeInverted:
        return screen.height - flickable.mapToItem(null, flickable.x, flickable.y).y

    case Screen.Portrait:
        return flickable.mapToItem(null, flickable.x, flickable.y).x

    case Screen.PortraitInverted:
        return screen.width - flickable.mapToItem(null, flickable.x, flickable.y).x
    }
}

function getMargin() {
    switch(screen.currentOrientation) {
    case Screen.Landscape:
    case Screen.LandscapeInverted:
        return 40
    case Screen.Portrait:
    case Screen.PortraitInverted:
        return 48
    }

    return 0
}

function repositionFlickable(animation) {
    inputContext.updateMicroFocus()
    var mf = inputContext.microFocus
    if(mf.x == -1 && mf.y == -1)
        return

    var object = findFlickable(parent)

    if(object){
        var flickable = object

        // Specifies area from bottom and top when repositioning should be triggered
        var margin = getMargin()

        var newContentY = flickable.contentY
        var flickableY = locateFlickableY(flickable)

        switch(screen.currentOrientation) {
        case Screen.Landscape:
            if(flickableY + flickable.height  - mf.height - margin < mf.y) {
                // Find dY just to make textfield visible
                var dY = mf.y - flickableY - flickable.height
                // Center textfield
                dY += flickable.height / 2
                newContentY += dY
            } else if(flickableY + margin > mf.y) {
                var dY = flickableY - mf.y
                dY += flickable.height / 2
                newContentY -= dY
            }

            break

        case Screen.LandscapeInverted:
            var invertedMfY = screen.displayHeight - mf.y - mf.height
            if(flickable.y + flickable.height - mf.height < invertedMfY) {
                newContentY += invertedMfY - flickable.y - flickable.height + mf.height
            } else if(flickable.y + topMargin > invertedMfY) {
                newContentY -= flickable.y - invertedMfY
            }

            break

        case Screen.Portrait:
            if(flickableY + flickable.height - mf.width - margin < mf.x) {
                var dY = mf.x - flickableY - flickable.height
                dY += flickable.height / 2
                newContentY += dY
            } else if(flickableY + margin > mf.x){
                var dY = flickableY - mf.x
                dY -= flickable.height / 2
                newContentY += dY
            }

            break

        case Screen.PortraitInverted:
            var invertedMfX = screen.displayWidth - mf.x - mf.width
            if(flickable.y + flickable.height - mf.width < invertedMfX) {
                newContentY += invertedMfX - flickable.y - flickable.height + mf.width
            } else if(flickable.y + topMargin > invertedMfX){
                newContentY -= flickable.y - invertedMfX
            }

            break
        }

        // If overpanned, set contentY to max possible value (reached bottom)
        if(newContentY > flickable.contentHeight - flickable.height)
            newContentY = flickable.contentHeight - flickable.height

        // If overpanned, set contentY to min possible value (reached top)
        if(newContentY < 0)
            newContentY = 0

        if(newContentY != flickable.contentY) {
            animateContentY(animation, flickable, newContentY)
        }
    }
}

function previousWordStart(pos) {
    var ret = pos;

    if (ret && atWordSeparator(ret - 1)) {
        ret--;
        while (ret && atWordSeparator(ret - 1))
            ret--;
    } else {
        while (ret && !atSpace(ret - 1) && !atWordSeparator(ret - 1))
            ret--;
    }

    return ret;
}

function nextWordEnd(pos) {
    var ret = pos;
    var len = root.text.length;

    if (ret < len && atWordSeparator(ret)) {
        ret++;
        while (ret < len && atWordSeparator(ret))
            ret++;
    } else {
        while (ret < len && !atSpace(ret) && !atWordSeparator(ret))
            ret++;
    }

    return ret;
}

function atSpace(pos) {
    var c = root.text.charAt(pos);
    return c == ' '
           || c == '\t'
           || c == '\n'
           ;
}

function atWordSeparator(pos) {
    switch (root.text.charAt(pos)) {
    case '.':
    case ',':
    case '?':
    case '!':
    case '@':
    case '#':
    case '$':
    case ':':
    case ';':
    case '-':
    case '<':
    case '>':
    case '[':
    case ']':
    case '(':
    case ')':
    case '{':
    case '}':
    case '=':
    case '/':
    case '+':
    case '%':
    case '&':
    case '^':
    case '*':
    case '\'':
    case '"':
    case '`':
    case '~':
    case '|':
        return true;
    default:
        return false;
    }
}

var MIN_UPDATE_INTERVAL = 30
var lastUpdateTime
function filteredInputContextUpdate() {
    if (Date.now() - lastUpdateTime > MIN_UPDATE_INTERVAL || !lastUpdateTime) {
        inputContext.update();
        lastUpdateTime = Date.now();
    }
}
