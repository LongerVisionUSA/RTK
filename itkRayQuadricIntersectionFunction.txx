namespace itk
{

template < class TCoordRep, unsigned int VBoxDimension >
RayQuadricIntersectionFunction<TCoordRep, VBoxDimension>
::RayQuadricIntersectionFunction():
m_A(0.), m_B(0.), m_C(0.), m_D(0.), m_E(0.),
m_F(0.), m_G(0.), m_H(0.), m_I(0.), m_J(0.)
{
}

template < class TCoordRep, unsigned int VBoxDimension >
bool
RayQuadricIntersectionFunction<TCoordRep, VBoxDimension>
::Evaluate( const VectorType& rayDirection )
{
  m_RayDirection = rayDirection;

  // http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter4.htm
  TCoordRep Aq = m_A*rayDirection[0]*rayDirection[0] +
                 m_B*rayDirection[1]*rayDirection[1] +
                 m_C*rayDirection[2]*rayDirection[2] +
                 m_D*rayDirection[0]*rayDirection[1] +
                 m_E*rayDirection[0]*rayDirection[2] +
                 m_F*rayDirection[1]*rayDirection[2];
  TCoordRep Bq = 2*(m_A*m_RayOrigin[0]*rayDirection[0] +
                    m_B*m_RayOrigin[1]*rayDirection[1] +
                    m_C*m_RayOrigin[2]*rayDirection[2]) +
                 m_D*(m_RayOrigin[0]*rayDirection[1] + m_RayOrigin[1]*rayDirection[0]) +
                 m_E*m_RayOrigin[0]*rayDirection[2] +
                 m_F*(m_RayOrigin[1]*rayDirection[2] + rayDirection[1]*m_RayOrigin[2]) +
                 m_G*rayDirection[0] +
                 m_H*rayDirection[1] +
                 m_I*rayDirection[2];
  TCoordRep Cq = m_A*m_RayOrigin[0]*m_RayOrigin[0] +
                 m_B*m_RayOrigin[1]*m_RayOrigin[1] +
                 m_C*m_RayOrigin[2]*m_RayOrigin[2] +
                 m_D*m_RayOrigin[0]*m_RayOrigin[1] +
                 m_E*m_RayOrigin[0]*m_RayOrigin[2] +
                 m_F*m_RayOrigin[1]*m_RayOrigin[2] +
                 m_G*m_RayOrigin[0] +
                 m_H*m_RayOrigin[1] +
                 m_I*m_RayOrigin[2] +
                 m_J;

  const TCoordRep zero = itk::NumericTraits<TCoordRep>::ZeroValue();
  if(Aq==zero)
    {
    m_NearestDistance = -Cq/Bq;
    m_FarthestDistance = itk::NumericTraits<TCoordRep>::max();
    }
  else
    {
    TCoordRep discriminant = Bq*Bq-4*Aq*Cq;
    if(discriminant<zero)
      return false;
    m_NearestDistance  = (-Bq-sqrt(discriminant))/(2*Aq);
    m_FarthestDistance = (-Bq+sqrt(discriminant))/(2*Aq);

    // The following condition is equivant to but assumed to be faster
    //if( vcl_abs(m_NearestDistance)>vcl_abs(m_FarthestDistance) )
    if( (m_NearestDistance-m_FarthestDistance)*(m_NearestDistance+m_FarthestDistance)>0. )
      std::swap(m_NearestDistance, m_FarthestDistance);
    }
  return true;
}

} // namespace itk