program vecsum
    implicit none
    real(8), dimension(:), allocatable :: x
    real(8), dimension(:), allocatable :: y
    real(8), dimension(:), allocatable :: z
    real(8) :: sum = 0
    real T1,T2
    integer :: n = 100000000
    integer :: i
   
    allocate(x(n))
    allocate(y(n))
    allocate(z(n))

    do i=1,n
       x(i) = sin(i*1D0)*sin(i*1D0)
       y(i) = cos(i*1D0)*cos(i*1D0)
       z(i) = 0
    enddo
    
    CALL CPU_TIME(T1)
    CALL vecadd(x,y,n,z)
    CALL CPU_TIME(T2)

    do i=1,n
       sum = sum + z(i)
    enddo
    print *, "Time(sec): ", T2-T1
    print *, "Vector Sum = ",sum/n
    deallocate(x)
    deallocate(y)
    deallocate(z)
end program vecsum

subroutine vecadd(x,y,n,z)
  ! compute vector addition f = x + y of size n vector
  implicit none
  integer, intent(in) :: n
  real(8), dimension(n), intent(in) :: x
  real(8), dimension(n), intent(in) :: y
  real(8), dimension(n), intent(out) :: z
  integer :: i
  do i=1,n
     z(i) = x(i)+y(i)
  enddo
end subroutine vecadd
