program dgemm
implicit none
   integer :: n 
   real(8), dimension(:,:), allocatable :: x
   real(8), dimension(:,:), allocatable :: y
   real(8), dimension(:,:), allocatable :: z
   real(8) :: sum = 0
    real T1,T2
    
    integer :: i,j,k,run
    
do run=1000,2000,200  
    T1 = 0.0
    T2 = 0.0
    n = run
    print *,"Matrix Multiplication for ", n,"*",n
    allocate(x(n,n))
    allocate(y(n,n))
    allocate(z(n,n))

    do i=1,n
       do j=1,n
          x(i,j) = sin(i*1D0)*sin(j*1D0)
          y(i,j) = cos(i*1D0)*cos(j*1D0)
          z(i,j) = 0
       enddo
    enddo

    CALL CPU_TIME(T1)
    do i=1,n
       do j=1,n
          sum = 0
          do k=1,n
             sum = x(i,k)*y(k,j)
          enddo
          z(i,j) = sum
       enddo
    enddo
    CALL CPU_TIME(T2)

    print *,"Time (sec):", T2-T1
    print *,"Total Flops Calculated: ", abs(n*n*n)
    print *,"Mflops/sec: ", abs((n*n*n/1000000))/abs(T2-T1)
    print *,"---------------------------------------------"
    deallocate(x)
    deallocate(y)
    deallocate(z)
enddo
end program dgemm
