program vecsum
      implicit none
      integer :: n = 100*1000*1000
      real(8), dimension(n) :: x
      real(8), dimension(n) :: y
      real(8), dimension(n) :: z
      real(8) :: sum = 0
      real T1,T2

      integer :: i
   
      do i=1,n
         x(i) = sin(i*1D0)*sin(i*1D0)
         y(i) = cos(i*1D0)*cos(i*1D0)
         z(i) = 0*1D0
      enddo

      CALL CPU_TIME(T1)
      CALL vecadd(x,y,n,z)
      CALL CPU_TIME(T2)
    
    
      print *, "Time(sec): ", T2-T1

      do i=1,n
         sum = sum + z(i)
      enddo

        print *, "Vector Sum for Z: ", sum/n
end program vecsum

subroutine vecadd(x,y,n,z)
  ! compute vector addition f = x + y of size n vector

      implicit none
      integer, intent(in) :: n
  real(8), dimension(n), intent(in) :: x
  real(8), dimension(n), intent(in) :: y
  real(8), dimension(n), intent(inout) :: z
  integer :: i
!$acc kernels copyin(x(1:n),y(1:n)), copy(z(1:n))
!$acc loop 
  do i=1,n
     z(i) = x(i)+y(i)
  enddo
!$acc end kernels
end subroutine vecadd
