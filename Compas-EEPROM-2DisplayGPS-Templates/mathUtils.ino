
  double caculaDistancia(double latIni,double lonIni,double lat,double lon){
    double radio = 6372.795;
    double dlat= latIni-lat;
    double dlon= lonIni-lon;
    double a=(sin(dlat/2)*sin(dlat/2))+(cos(latIni)*cos(lat)*sin(dlon/2)*sin(dlon/2));
    double c=2*atan2(sqrt(a),sqrt(1-a));
    double distancia=radio*c;
    return distancia;
  }

  double deg2rad(double deg){
    double rad= deg*PI/180;
    return rad;
  }
