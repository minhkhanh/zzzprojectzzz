﻿using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.Drawing;

namespace vCards
{
    public abstract class BmpControl: MyControl
    {
        protected IBitmap bmpBkgr;

        public BmpControl(Rectangle regn, string bmpPath, IGraphics igp)
            : base(regn)
        {
            bmpBkgr = igp.CreateBitmap(bmpPath, true);
        }

        public override void DrawBackground(IGraphics igp)
        {
            igp.DrawBitmap(region, bmpBkgr);
        }

        public override void DoDrawing(IGraphics igp)
        {
            DrawBackground(igp);
        }
    }
}
