/*---------------------------------------------------------------------------*\
     ██╗████████╗██╗  ██╗ █████╗  ██████╗ █████╗       ███████╗██╗   ██╗
     ██║╚══██╔══╝██║  ██║██╔══██╗██╔════╝██╔══██╗      ██╔════╝██║   ██║
     ██║   ██║   ███████║███████║██║     ███████║█████╗█████╗  ██║   ██║
     ██║   ██║   ██╔══██║██╔══██║██║     ██╔══██║╚════╝██╔══╝  ╚██╗ ██╔╝
     ██║   ██║   ██║  ██║██║  ██║╚██████╗██║  ██║      ██║      ╚████╔╝
     ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝      ╚═╝       ╚═══╝

 * In real Time Highly Advanced Computational Applications for Finite Volumes
 * Copyright (C) 2017 by the ITHACA-FV authors
-------------------------------------------------------------------------------
  License
  This file is part of ITHACA-FV
  ITHACA-FV is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  ITHACA-FV is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public License
  along with ITHACA-FV. If not, see <http://www.gnu.org/licenses/>.
\*---------------------------------------------------------------------------*/

scalar residual = 1;
scalar uresidual = 1;
Vector<double> uresidual_v(0, 0, 0);
scalar presidual = 1;
scalar fluxresidual = 1;
scalar prec1residual = 1;
scalar prec2residual = 1;
scalar prec3residual = 1;
scalar prec4residual = 1;
scalar prec5residual = 1;
scalar prec6residual = 1;
scalar prec7residual = 1;
scalar prec8residual = 1;
scalar Tresidual = 1;
scalar dec1residual = 1;
scalar dec2residual = 1;
scalar dec3residual = 1;
scalar csolve = 0;


// Variable that can be changed
turbulence->read();
std::ofstream res_os;
res_os.open("./ITHACAoutput/Offline/residuals", std::ios_base::app);
std::string equation;

#if defined(OFVER) && (OFVER == 6)

while (simple.loop(runTime) && residual > tolerance && csolve < maxIter )
#else
while (simple.loop() && residual > tolerance && csolve < maxIter )
#endif
{
    Info << "Time = " << runTime.timeName() << nl << endl;
    {
#include "UEqn.H"
#include "TEqn.H"
        TEqn.relax();
        Tresidual = TEqn.solve().initialResidual();
#include "pEqn.H"
#include "updateConsts.H"
#include "DiffEqn.H"
#include "precEqns.H"
#include "decEqns.H"
#include "updateK.H"
        flux_old = flux;
        TEqn.relax();
        Tresidual = TEqn.solve().initialResidual();
        scalar C = 0;

        for (label i = 0; i < 3; i++)
        {
            if (C < uresidual_v[i])
            {
                C = uresidual_v[i];
            }
        }

        uresidual = C;
        residual = max(presidual, uresidual);
        residual = max(residual, fluxresidual);
        residual = max(residual, prec1residual);
        residual = max(residual, prec2residual);
        residual = max(residual, prec3residual);
        residual = max(residual, prec4residual);
        residual = max(residual, prec5residual);
        residual = max(residual, prec6residual);
        residual = max(residual, prec7residual);
        residual = max(residual, prec8residual);
        residual = max(residual, Tresidual);
        residual = max(residual, dec1residual);
        residual = max(residual, dec2residual);
        residual = max(residual, dec3residual);
        Info << "\nResidual: " << residual << endl << endl;
    }

    laminarTransport.correct();
    turbulence->correct();
    powerDens = (1 - decbetaTot) * flux * SP1_0 + (decLam1 * dec1 + decLam2 * dec2 +
                decLam3 * dec3);
    csolve = csolve + 1;
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
         << "  ClockTime = " << runTime.elapsedClockTime() << " s"
         << nl << endl;
    Info << "\nIteration number: " << csolve << endl << endl;
}

res_os << residual << std::endl;
res_os.close();
runTime.setTime(runTime.startTime(), 0);
ITHACAstream::exportSolution(U, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(p, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(flux, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(prec1, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(prec2, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(prec3, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(prec4, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(prec5, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(prec6, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(prec7, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(prec8, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(T, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(dec1, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(dec2, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(dec3, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(powerDens, name(counter),
                             "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(v, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(D, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(NSF, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(A, name(counter), "./ITHACAoutput/Offline/");
ITHACAstream::exportSolution(SP, name(counter), "./ITHACAoutput/Offline/");
Ufield.append(U.clone());
Pfield.append(p.clone());
Fluxfield.append(flux.clone());
Prec1field.append(prec1.clone());
Prec2field.append(prec2.clone());
Prec3field.append(prec3.clone());
Prec4field.append(prec4.clone());
Prec5field.append(prec5.clone());
Prec6field.append(prec6.clone());
Prec7field.append(prec7.clone());
Prec8field.append(prec8.clone());
Tfield.append(T.clone());
Dec1field.append(dec1.clone());
Dec2field.append(dec2.clone());
Dec3field.append(dec3.clone());
vFields.append(v.clone());
DFields.append(D.clone());
NSFFields.append(NSF.clone());
AFields.append(A.clone());
SPFields.append(SP.clone());








