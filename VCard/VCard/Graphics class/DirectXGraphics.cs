﻿using System;
using System.Drawing;
using System.Windows.Forms;
using Microsoft.WindowsMobile.DirectX;
using Microsoft.WindowsMobile.DirectX.Direct3D;
namespace VCard
{
    public class DirectXGraphics : IGraphics
    {
        /// <summary>
        /// The rendering device for D3D
        /// </summary>
        Device device;

        /// <summary>
        /// A buffer of vertex data for colored (non-textured) drawing
        /// operations
        /// </summary>
        VertexBuffer vbCol;

        /// <summary>
        /// A buffer of vertex data for textured drawing operations
        /// </summary>
        VertexBuffer vbTex;

        /// <summary>
        /// The fixed height of our rendering area
        /// </summary>
        private static int Width = 240;

        /// <summary>
        /// The fixed width of our rendering area
        /// </summary>
        private static int Height = 320;

        public DirectXGraphics(Control ctrl)
        {
            Pool vertexBufferPool;

            // Setup D3D parameters
            PresentParameters presentParams = new PresentParameters();
#if !DESKTOP
            presentParams.Windowed = false;
            presentParams.SwapEffect = SwapEffect.Discard;
            presentParams.BackBufferCount = 1;
            presentParams.BackBufferFormat = Format.R5G6B5;
            presentParams.BackBufferWidth = Width;
            presentParams.BackBufferHeight = Height;
            device = new Device(0, DeviceType.Default, ctrl,
                CreateFlags.None, presentParams);
#else
                presentParams.Windowed = true;
                presentParams.SwapEffect = SwapEffect.Discard;
                device = new Device(0, DeviceType.Hardware, ctrl,
                    CreateFlags.SoftwareVertexProcessing, presentParams);
#endif
            device.RenderState.AlphaTestEnable = true;
            device.RenderState.AlphaFunction = Compare.NotEqual;
            device.RenderState.ReferenceAlpha = 0;

#if !DESKTOP
            // Get the device capabilities

            Caps caps;
            caps = device.DeviceCaps;

            if (caps.SurfaceCaps.SupportsVidVertexBuffer)
                vertexBufferPool = Pool.VideoMemory;
            else
                vertexBufferPool = Pool.SystemMemory;
#else
                vertexBufferPool = Pool.SystemMemory;
#endif

            vbCol = new VertexBuffer(
                typeof(CustomVertex.TransformedColored), 4, device,
                0, CustomVertex.TransformedColored.Format, vertexBufferPool);
            vbTex = new VertexBuffer(
                typeof(CustomVertex.TransformedTextured), 4, device,
                0, CustomVertex.TransformedTextured.Format,
                vertexBufferPool);

            // Clear the backbuffer to a blue color 
            device.Clear(ClearFlags.Target, System.Drawing.Color.Blue,
                1.0f, 0);
            // Begin the scene
            device.BeginScene();
        }


        #region IGraphics Members

        /// <summary>
        /// Gets the width of the screen.
        /// </summary>
        public int ScreenWidth
        {
            get
            {
                return Width;
            }
        }

        /// <summary>
        /// Gets the height of the screen.
        /// </summary>
        public int ScreenHeight
        {
            get
            {
                return Height;
            }
        }

        /// <summary>
        /// Draw the current cell of the animation to the back buffer.
        /// </summary>
        /// <param name="x">X destination of the draw</param>
        /// <param name="y">Y destination of the draw</param>
        /// <param name="animation">Animation to be drawn</param>
        public void DrawAnimation(int x, int y, Animation animation)
        {
            DrawBitmap(x, y, animation.Region, animation.Image);
        }

        /// <summary>
        /// Draw the bitmap to the back buffer.
        /// </summary>
        /// <param name="x">X destination of the draw</param>
        /// <param name="y">Y destination of the draw</param>
        /// <param name="sourceRegion">Source region of the draw</param>
        /// <param name="bmp">Bitmap to be drawn</param>
        public void DrawBitmap(int x, int y, Rectangle sourceRegion,
            IBitmap bmp)
        {

            // make sure the function is passed an appropriate implementation
            DirectXBitmap dxBitmap = null;
            try
            {
                dxBitmap = (DirectXBitmap)bmp;
            }
            catch (InvalidCastException e)
            {
                throw new ApplicationException(
                    "The bitmap given was not created by" +
                    "this class' CreateBitmap() method.", e);
            }

            // Clip the regions to the screen
            if (!ValidateRegions(ref x, ref y, ref sourceRegion))
                return;

            // determine texture coordinates so that
            // the there is a 1-1 mapping between texels and screen pixels
            Rectangle dest = new Rectangle(x, y, sourceRegion.Width,
                sourceRegion.Height);
            RectangleF textureRect = new RectangleF(
                ((float)sourceRegion.X + 0.5F) / dxBitmap.Width,
                ((float)sourceRegion.Y + 0.5F) / dxBitmap.Height,
                ((float)sourceRegion.Width + 0.5F) / dxBitmap.Width,
                ((float)sourceRegion.Height + 0.5F) / dxBitmap.Height);


            // load the vertex buffer with data for the two triangles
            // in the rectangle
            GraphicsStream stm = vbTex.Lock(0, 0, 0);
            CustomVertex.TransformedTextured[] verts =
                new CustomVertex.TransformedTextured[4];

            if ((drawOptions & DrawOptions.BlitMirrorLeftRight) == 0)
            {
                verts[0].X = dest.Right;
                verts[0].Y = dest.Top;
                verts[0].Z = 0.5f;
                verts[0].Rhw = 1;
                verts[0].Tu = textureRect.Right;
                verts[0].Tv = textureRect.Top;
                verts[1].X = dest.Right;
                verts[1].Y = dest.Bottom;
                verts[1].Z = 0.5f;
                verts[1].Rhw = 1;
                verts[1].Tu = textureRect.Right;
                verts[1].Tv = textureRect.Bottom;
                verts[2].X = dest.Left;
                verts[2].Y = dest.Top;
                verts[2].Z = 0.5f;
                verts[2].Rhw = 1;
                verts[2].Tu = textureRect.Left;
                verts[2].Tv = textureRect.Top;
                verts[3].X = dest.Left;
                verts[3].Y = dest.Bottom;
                verts[3].Z = 0.5f;
                verts[3].Rhw = 1;
                verts[3].Tu = textureRect.Left;
                verts[3].Tv = textureRect.Bottom;
            }
            else
            {
                verts[0].X = dest.Right;
                verts[0].Y = dest.Top;
                verts[0].Z = 0.5f;
                verts[0].Rhw = 1;
                verts[0].Tu = textureRect.Left;
                verts[0].Tv = textureRect.Top;
                verts[1].X = dest.Right;
                verts[1].Y = dest.Bottom;
                verts[1].Z = 0.5f;
                verts[1].Rhw = 1;
                verts[1].Tu = textureRect.Left;
                verts[1].Tv = textureRect.Bottom;
                verts[2].X = dest.Left;
                verts[2].Y = dest.Top;
                verts[2].Z = 0.5f;
                verts[2].Rhw = 1;
                verts[2].Tu = textureRect.Right;
                verts[2].Tv = textureRect.Top;
                verts[3].X = dest.Left;
                verts[3].Y = dest.Bottom;
                verts[3].Z = 0.5f;
                verts[3].Rhw = 1;
                verts[3].Tu = textureRect.Right;
                verts[3].Tv = textureRect.Bottom;
            }

            stm.Write(verts);
            vbTex.Unlock();

            // bind the texture to next drawing operation
            device.SetTexture(0, dxBitmap.Texture);

            // bind the vertex data to the next drawing operation
            device.SetStreamSource(0, vbTex, 0);
#if DESKTOP
                    device.VertexFormat =
                        CustomVertex.TransformedTextured.Format;
#endif
            // perform the draw
            device.DrawPrimitives(PrimitiveType.TriangleStrip, 0, 2);
        }

        /// <summary>
        /// Draw a filled rectangle to the back buffer.
        /// </summary>
        /// <param name="r">Rectangle to be filled</param>
        /// <param name="c">Color of rectangle</param>
        public void DrawFilledRect(Rectangle r, Color c)
        {
            // fill the vertex buffer with the correct vertices
            GraphicsStream stm = vbCol.Lock(0, 0, 0);
            CustomVertex.TransformedColored[] verts =
                new CustomVertex.TransformedColored[4];

            verts[0].X = r.Left;
            verts[0].Y = r.Bottom;
            verts[0].Z = 0.5f;
            verts[0].Rhw = 1;
            verts[0].Color = c.ToArgb();
            verts[1].X = r.Left;
            verts[1].Y = r.Top;
            verts[1].Z = 0.5f;
            verts[1].Rhw = 1;
            verts[1].Color = c.ToArgb();
            verts[2].X = r.Right;
            verts[2].Y = r.Top;
            verts[2].Z = 0.5f;
            verts[2].Rhw = 1;
            verts[2].Color = c.ToArgb();
            verts[3].X = r.Right;
            verts[3].Y = r.Bottom;
            verts[3].Z = 0.5f;
            verts[3].Rhw = 1;
            verts[3].Color = c.ToArgb();
            stm.Write(verts);
            vbCol.Unlock();

            // bind the vertex data to the next drawing operation
            device.SetStreamSource(0, vbCol, 0);
#if DESKTOP
                device.VertexFormat = CustomVertex.TransformedColored.Format;
#endif
            // draw the triangles
            device.DrawPrimitives(PrimitiveType.TriangleFan, 0, 2);
        }


        /// <summary>
        /// Flip the back buffer to the display.
        /// </summary>
        public void Flip()
        {
            if (device == null)
                return;

            // End the current rendering
            device.EndScene();
            device.Present();

            // Start a new rendering...
            // Clear the backbuffer to a blue color 
            device.Clear(ClearFlags.Target, System.Drawing.Color.Blue,
                1.0f, 0);
            // Begin the scene
            device.BeginScene();
        }

        /// <summary>
        /// Draw the string to the back buffer.
        /// </summary>
        /// <param name="x">X destination of text</param>
        /// <param name="y">Y destination of text</param>
        /// <param name="text">Text to be displayed</param>
        /// <param name="color">Color of text</param>
        /// <param name="font">Font to be used</param>
        /// <param name="options">Font draw options</param>
        public void DrawText(int x, int y, string text, Color color,
            IFont font, FontDrawOptions options)
        {
            // make sure the function is passed an appropriate implementation
            DirectXFont dxFont = null;
            try
            {
                dxFont = (DirectXFont)font;
            }
            catch (InvalidCastException e)
            {
                throw new ApplicationException(
                    "The font given was not created by" +
                    "this class' CreateFont() method.", e);
            }

            // determine the rectangle to draw in
            Rectangle rect = new Rectangle();
#if DESKTOP
                dxFont.Font.DrawText(null, text, ref rect,
                    DrawTextFormat.CalculateRect, color);
#else
            dxFont.Font.MeasureString(null, text, ref rect, 0);
#endif
            rect.Y = y;
            rect.X = 0;
            rect.Width = ScreenWidth;

            // set options for the DrawText call
            DrawTextFormat drawOptions;
            if (options == FontDrawOptions.DrawTextCenter)
                drawOptions = DrawTextFormat.Center;
            else if (options == FontDrawOptions.DrawTextLeft)
                drawOptions = DrawTextFormat.Left;
            else
                drawOptions = DrawTextFormat.Right;

            dxFont.Font.DrawText(null, text, rect, drawOptions, color);
        }

        /// <summary>
        /// Draw options used for drawing bitmaps.
        /// </summary>
        DrawOptions drawOptions = 0;

        /// <summary>
        /// Set the current draw mode.
        /// </summary>
        /// <param name="options">options to be set</param>
        public void SetDrawOptions(DrawOptions options)
        {
            drawOptions |= options;
        }

        /// <summary>
        /// Clear the current draw mode of the specified options.
        /// </summary>
        /// <param name="options">options to be cleared</param>
        public void ClearDrawOptions(DrawOptions options)
        {
            drawOptions &= ~options;
        }

        /// <summary>
        /// Validate draw regions be clipping them to the screen.
        /// </summary>
        /// <param name="x">X destination</param>
        /// <param name="y">Y destination</param>
        /// <param name="sourceRegion">Source region</param>
        /// <returns>true if any part of the destination is drawable
        /// (on-screen), false otherwise</returns>
        public bool ValidateRegions(ref int x, ref int y, ref Rectangle sourceRegion)
        {
            Rectangle clippedSrc = new Rectangle(sourceRegion.X, sourceRegion.Y,
                sourceRegion.Width, sourceRegion.Height);
            if (x < 0)
            {
                if (x + sourceRegion.Width < 0) return false;
                clippedSrc.Width += x;
                clippedSrc.X -= x;
                x = 0;
            }
            else if (x >= ScreenWidth)
            {
                return false;
            }

            if (y < 0)
            {
                if (y + sourceRegion.Height < 0) return false;
                clippedSrc.Height += y;
                clippedSrc.Y -= y;
                y = 0;
            }
            else if (y >= ScreenHeight)
            {
                return false;
            }

            if (x + sourceRegion.Width > ScreenWidth)
            {
                clippedSrc.Width -= (x + sourceRegion.Width) - ScreenWidth;
            }

            if (y + sourceRegion.Height > ScreenHeight)
            {
                clippedSrc.Height -= (y + sourceRegion.Height) - ScreenHeight;
            }

            if ((drawOptions & DrawOptions.BlitMirrorLeftRight) != 0)
            {
                sourceRegion.X = sourceRegion.X + sourceRegion.Width -
                    (clippedSrc.X - sourceRegion.X) - clippedSrc.Width;
            }
            else
            {
                sourceRegion.X = clippedSrc.X;
            }

            sourceRegion.Width = clippedSrc.Width;
            sourceRegion.Y = clippedSrc.Y;
            sourceRegion.Height = clippedSrc.Height;

            return true;
        }

        /// <summary>
        /// Creates a bitmap compatible with this graphics device
        /// </summary>
        /// <param name="fileName">The file to load the image from</param>
        /// <param name="transparent">True if the image should be drawn
        /// with alpha transparency</param>
        /// <returns>A bitmap compatible with this graphics device</returns>
        public IBitmap CreateBitmap(string fileName, bool transparent)
        {
            return new DirectXBitmap(fileName, device, transparent);
        }


        /// <summary>
        /// Creates a font object compatible with this graphics device
        /// </summary>
        /// <param name="fontName"></param>
        /// <returns>A font object compatible with this graphics device
        /// </returns>
        public IFont CreateFont(string fontName)
        {
            return new DirectXFont(fontName, device);
        }

        #endregion

        #region IDisposable Members

        public void Dispose()
        {
            device.Dispose();
        }

        #endregion
        public void DrawImageTransparent(IBitmap image, Rectangle destRect)
        {

        }
        public void DrawImageTransparent(IBitmap image, Rectangle srcRect, int x, int y)
        {

        }
    }
}