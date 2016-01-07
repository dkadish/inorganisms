RGB hsv2rgb(unsigned char h) {
  RGB rgb;
  unsigned char region, fpart, p, q, t;

  /* make hue 0-5 */
  region = h / 43;
  /* find remainder part, make it from 0-255 */
  fpart = (h - (region * 43)) * 6;

  /* calculate temp vars, doing integer multiplication */
  p = 0 >> 8;
  q = (255 * (255 - ((255 * fpart) >> 8))) >> 8;
  t = (255 * (255 - ((255 * (255 - fpart)) >> 8))) >> 8;

  /* assign temp vars based on color cone region */
  switch (region) {
    case 0:
      rgb.r = 255; rgb.g = t; rgb.b = p; break;
    case 1:
      rgb.r = q; rgb.g = 255; rgb.b = p; break;
    case 2:
      rgb.r = p; rgb.g = 255; rgb.b = t; break;
    case 3:
      rgb.r = p; rgb.g = q; rgb.b = 255; break;
    case 4:
      rgb.r = t; rgb.g = p; rgb.b = 255; break;
    default:
      rgb.r = 255; rgb.g = p; rgb.b = q; break;
  }

  return rgb;
}
